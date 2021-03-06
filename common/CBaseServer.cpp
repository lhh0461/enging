/**
 * 网络模型：
 * 一个主线程接受到连接后把连接socket就扔到子线程
 *
 */
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "CBaseServer.h"
#include "CLog.h"
#include "NetTool.h"
#include "Cmd.h"
#include "CPackage.h"
#include "CRpc.h"
#include "Common.h"
#include "XEngine.h"
#include "CConfigParser.h"
#include "CConnState.h"

namespace XEngine
{

CBaseServer *g_Server;

CBaseServer::CBaseServer(SERVER_TYPE server_type)
    :m_EpollFd(0), m_ListenFd(0), m_ServerType(server_type)
{
    m_Config = new CConfigParser();
    m_Rpc = new CRpc();

    if (m_Config->Parser("./etc/config.ini") == false) {
        LOG_ERROR("server init fail");
        exit(1);
    }
}

CBaseServer::~CBaseServer()
{
    if (m_Config) {
        delete m_Config;
    }
    if (m_Rpc) {
        delete m_Rpc;
    }
}

int CBaseServer::Init()
{
    m_ClusterId = atoi(m_Config->GetConfig("global", "CLUSTER_ID").c_str());
    m_Rpc->Init(m_Config->GetConfig("global", "RPC_PATH"));
     

    m_EpollFd = epoll_create(MAX_EVENT); 
    
    LOG_INFO("server init success");

    PyImport_AppendInittab("XEngine", PyInit_XEngine);

    Py_Initialize();
    PyRun_SimpleString("import sys\nsys.path.append(\"./scripts\")");
    PyImport_ImportModule("XEngine");

    //都主动去连CENTERD
    if (m_ServerType != SERVER_TYPE_CENTERD) {
        if (ConnectToServer(SERVER_TYPE_CENTERD, CENTERD_ID, m_Config->GetConfig("centerd", "IP").c_str(), 
                    atoi(m_Config->GetConfig("centerd", "PORT").c_str())))
        {
            LOG_ERROR("connect to centerd fail");
            exit(1);
        }
    }
}

void CBaseServer::Run()
{
    struct epoll_event events[MAX_EVENT];
    for (;;)
    {
        int nfds = epoll_wait(m_EpollFd, events, MAX_EVENT, 500);
        for(int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == m_ListenFd) {
                HandleNewConnection();
            } else { 
                auto it = m_ConnStat.find(events[i].data.fd);
                if (it == m_ConnStat.end()) {
                    continue; 
                }
                CConnState *pConnState = it->second;
                if (events[i].events & EPOLLIN) {
                    HandleRecvMsg(pConnState);
                }
                else if (events[i].events & EPOLLOUT) {
                    HandleWriteMsg(pConnState);
                }
            }
        }
        //处理所有的包
        HandlePackage();
        //TODO 遍历所有的连接，然后发包
        SendPackage();
        //处理定时器
    }
}

void CBaseServer::AddListenFd(std::string ip, int port)
{
    int fd = Listen(ip.c_str(), port, 1024);
    if (fd < 0) {
        LOG_ERROR("listen port fail;port=%d", port);
        exit(1);
    }

    m_ListenFd = fd;
    AddFdToEpoll(fd, EPOLLIN|EPOLLET);
    LOG_INFO("listen success.ip=%s,port=%d,m_ListenFd=%d", ip.c_str(), port, m_ListenFd);
}

int CBaseServer::HandleNewConnection()
{
    std::string ip;
    int port;
    int conn_fd = Accept(m_ListenFd, ip, port);
    if (conn_fd < 0) {
        LOG_ERROR("accept new fd fail,listen_fd=%d,ip=%s,port=%d,errstr=%s",m_ListenFd,ip.c_str(), port, strerror(errno));
        return 0;
    }

    SetBlock(conn_fd, 0, NULL);

    AddFdToEpoll(conn_fd, EPOLLIN|EPOLLET);

    CConnState *pconn = new CConnState(conn_fd, ip.c_str(),port, CONN_ACCEPTED_FLAG|CONN_CLIENT_FLAG);
    pconn->SetConnected(1);
    m_ConnStat.insert(std::make_pair(conn_fd, pconn));

    OnAcceptFdCallBack(pconn);
    LOG_INFO("accept new conn_fd=%d",conn_fd);

    return 0;
}

void CBaseServer::AddFdToEpoll(int fd, uint32_t events)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, fd, &ev);
}

void CBaseServer::ModifFdEpollEvents(int fd, uint32_t events)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(m_EpollFd, EPOLL_CTL_MOD, fd, &ev);
}

void CBaseServer::HandleRecvMsg(CConnState *conn)
{
    for (;;)
    {
        if (conn != NULL)
        {
            //剩余的的缓冲区
            char *pRecvCurBuf = conn->GetRecvCurBuf();
            int iRecvBufLeftSize = conn->GetRecvBufLeftSize();
            //读取数据到缓冲区，缓冲区当前buf位置、剩余buf的偏移
            size_t num = ::recv(conn->GetFd(), pRecvCurBuf, iRecvBufLeftSize, 0);
            if (num == (size_t)-1) {
                if (errno == EINTR) {
                    continue;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                } else {
                    LOG_ERROR("recv msg error");
                    break;
                }
            }
            else if (num > 0) {
                size_t iTotalLen = conn->GetRecvBufPos() + num;
                //记录已经处理的字节数
                size_t iCurrentPos = 0;
                //数据已经读取到缓冲区，缓冲区中可能包含多个包
                //逐个逐个去切包
                //跳出这个循环有两种可能，一种是包头还没读取，二是包没读取够
                for (;;) {
                    size_t iLeftDataLen = iTotalLen - iCurrentPos;
                    pRecvCurBuf = conn->GetRecvCurBuf() + iCurrentPos;
                    /////目前缓冲区中缓冲的总字节数
                    //conn->SetRecvBufLen(totallen);
                    //包头还没读取够
                    if (iLeftDataLen < sizeof(CPackage::PKG_HEADER_TYPE)) {
                        break;
                    }
                    //包还没读取够
                    size_t packsize = *(CPackage::PKG_HEADER_TYPE *)pRecvCurBuf;
                    if (iTotalLen < packsize) {
                        break;
                    }

                    LOG_DEBUG("CBaseServer::HandleRecvMsg iTotalLen=%d", iTotalLen);
                    CPackage *package = new CPackage(pRecvCurBuf + sizeof(CPackage::PKG_HEADER_TYPE), packsize);
                    this->AddRecvPack(package);
                    iCurrentPos += packsize;
                }
                if (iCurrentPos > 0 && iCurrentPos != iTotalLen) {
                    memmove(conn->GetRecvBuf(), conn->GetRecvCurBuf() + iCurrentPos, iTotalLen - iCurrentPos);
                }
                if ((int)num == iRecvBufLeftSize) {
                    //也许还有数据没读取完毕
                    continue;
                } else {
                    //数据已经读取完毕
                    break;
                }
            }
            else {
                //对端已经关闭连接
                //TODO 检查是否需要重连
                OnCloseFdCallBack(conn);
            }
        }
    }
}

void CBaseServer::HandleWriteMsg(CConnState *conn)
{
    if (!conn->IsConnected()) {
        int optval;
        socklen_t optlen = sizeof(optval);
        int res = getsockopt(conn->GetFd(), SOL_SOCKET, SO_ERROR, &optval, &optlen);
        if (res < 0 || optval) {
            LOG_ERROR("connect to server fail");
            delete conn;
            m_ConnStat.erase(conn->GetFd());
            return;
        }
        conn->SetConnected(1);
        OnConnectFdCallBack(conn);    
        LOG_DEBUG("connect fd success");
    } else {
        //TODO
    }
}

void CBaseServer::HandlePackage()
{
    while (!m_RecvPackList.empty()) {
        CPackage *package = m_RecvPackList.front();
        LOG_DEBUG("CBaseServer::HandlePackage package");
        m_RecvPackList.pop_front();
        int res = this->OnRpcCall(package);
        delete package;
        LOG_DEBUG("CBaseServer::HandlePackage package res=%d",res);
    }
}

void CBaseServer::SendPackage()
{
    auto it = m_ConnStat.begin();
    for (; it != m_ConnStat.end(); it++) {
        CConnState *conn = it->second;
        if (conn) {
            conn->SendPackage();
        }
    }
}

int CBaseServer::OnRpcCall(CPackage *package)
{
    CMD_ID cmd_id;
    package->UnPackCmd(cmd_id); 
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;
    int err_code = CmdDispatch(cmd_id, package);
    if (err_code != ERR_SUCCESS) {
        LOG_ERROR("OnRpcCall dispatch fail");
    }
    return err_code;
}

int CBaseServer::CmdDispatch(CMD_ID cmd_id, CPackage *package)
{
    switch(cmd_id) {
        case MSG_CMD_RPC:
            return m_Rpc->Dispatch(package->GetPkgConn(), package);
        default:
            break;
    }
    return ERR_UNKOWN_CMD;
}

int CBaseServer::ConnectToServer(SERVER_TYPE server_type, SERVER_ID server_id, const char *ip, int port)
{
    int conn_fd = Connect(ip, port, 0);
    if (conn_fd < 0) {
        LOG_ERROR("connect fd fail,ip=%s,port=%d",ip, port);
        return -1;
    }

    SetBlock(conn_fd, 0, NULL);

    AddFdToEpoll(conn_fd, EPOLLOUT);

    //创建连接状态
    CConnState *conn = new CConnState(conn_fd, ip, port, CONN_MAILBOX_FLAG|CONN_HARBOR_FLAG);
    conn->SetServerType(server_type);
    conn->SetServerId(server_id);
    m_ConnStat.insert(std::make_pair(conn_fd, conn));

    LOG_INFO("connect to server;server_type=%d,server_id=%d,ip=%s,port=%d",server_type,server_id,ip,port);
    return 0;
}

int CBaseServer::GetServerConnByType(SERVER_TYPE server_type, std::list<CConnState *> &conn_list)
{
    auto it = m_ServerId2Conn.begin();
    for (; it != m_ServerId2Conn.end(); it++) {
        CConnState *pConn = it->second;
        if (pConn) {
            if (pConn->GetServerType() == server_type) {
                conn_list.push_back(pConn);
            }
        }
    }
    return 0; 
}

//连接服务器成功回调
int CBaseServer::OnConnectFdCallBack(CConnState *conn)
{
    LOG_INFO("OnConnectFdCallBack");
    ModifFdEpollEvents(conn->GetFd(), EPOLLIN|EPOLLET);
     
    //如果是CENTERD，则发送注册信息
    if (conn->GetServerType() == SERVER_TYPE_CENTERD) {
        std::string pwd = m_Config->GetConfig("global", "CLUSTER_PWD");
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_SERVER_REGISTER);
        pack->PackUInt8(GetServerType());
        pack->PackString(pwd.c_str(), pwd.size());
        conn->PushSendPackList(pack);
        LOG_INFO("send register msg,pwd=%s", pwd.c_str());
    } else {
        //如果不是CENTERD，则同步我的服务器信息给对方
        std::string pwd = m_Config->GetConfig("global", "CLUSTER_PWD");
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_SYNC_SERVER_INFO);
        pack->PackUInt8(GetServerType());
        pack->PackString(pwd.c_str(), pwd.size());
        conn->PushSendPackList(pack);
    }
}

int CBaseServer::OnAcceptFdCallBack(CConnState *conn)
{
    
}

//重载用
int CBaseServer::OnCloseFdCallBack(CConnState *conn)
{
    
}

}

