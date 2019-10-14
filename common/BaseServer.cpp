#include <sys/epoll.h>

#include "BaseServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Package.h"
#include "Rpc.h"
#include "Common.h"
#include "XEngine.h"
#include "ConfigParser.h"
#include "ConnState.h"

using namespace std;

namespace XEngine
{

CBaseServer *g_Server;

CBaseServer::CBaseServer(SERVER_TYPE server_type)
    :m_EpollFd(0), m_ListenFd(0), m_ServerType(server_type)
{
    m_Config = new CConfigParser();
    m_Rpc = new CRpc();
}

CBaseServer::~CBaseServer()
{

}

void CBaseServer::Init()
{
    if (m_Config->Parser("./config.ini") == false) {
        LOG_ERROR("server init fail");
        exit(1);
    }

    m_Rpc->Init("./rpc/");

    std::string ip = m_Config->GetConfig("global", "IP");
    int port = atoi(m_Config->GetConfig("global", "PORT").c_str());

    m_EpollFd = epoll_create(MAX_EVENT); 
    
    int fd = Listen(ip.c_str(), port, 1024);
    if (fd < 0) {
        LOG_ERROR("listen port fail;port=%d", port);
        exit(1);
    }

    LOG_INFO("server init success");

    m_ListenFd = fd;
    AddFdToEpoll(fd);
    PyImport_AppendInittab("XEngine", PyInit_XEngine);

    Py_Initialize();
    PyRun_SimpleString("import sys\nsys.path.append(\"./scripts\")");
    PyImport_ImportModule("XEngine");

    //都主动去连CENTERD
    if (m_ServerType != SERVER_TYPE_CENTERD) {
        if (ConnectToServer(SERVER_TYPE_CENTERD, CENTERD_ID, m_Config->GetConfig("centerd", "IP").c_str(), atoi(m_Config->GetConfig("centerd", "PORT").c_str()))) {
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
            }
            else { 
                auto it = m_ConnStat.find(events[i].data.fd);
                if (it == m_ConnStat.end()) {
                    continue; 
                }
                CConnState *conn = it->second;
                if (events[i].events & EPOLLIN) {
                    HandleRecvMsg(conn);
                }
                else if (events[i].events & EPOLLOUT) {
                    HandleWriteMsg(conn);
                }
            }
        }
        HandlePackage();
    }
}

void CBaseServer::HandleNewConnection()
{
    std::string ip;
    int port;
    int conn_fd = Accept(m_ListenFd, ip, port);
    if (conn_fd < 0) {
        LOG_ERROR("accept new fd fail");
        return;
    }

    SetBlock(conn_fd, 0, NULL);

    AddFdToEpoll(conn_fd);

    CConnState *conn = new CConnState(0, 0, ip.c_str(),port, CONN_ACCEPTED_FLAG|CONN_CLIENT_FLAG);
    conn->SetConnected(1);
    m_ConnStat.insert(std::make_pair(conn_fd, conn));

    OnAcceptFdCallBack(conn);
}

void CBaseServer::AddFdToEpoll(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, fd, &ev);
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
            //读取数据到缓冲区
            size_t num = ::recv(conn->GetFd(), pRecvCurBuf, iRecvBufLeftSize, 0);
            if (num == (size_t)-1) {
                if (errno == EINTR) {
                    continue;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
                break;
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

                    CPackage *package = new CPackage(pRecvCurBuf + sizeof(CPackage::PKG_HEADER_TYPE), packsize);

                    std::list<CPackage *> *recvList = conn->GetRecvPackList(); 
                    recvList->push_back(package);

                    iCurrentPos += packsize;
                }
                if (iCurrentPos > 0 && iCurrentPos != iTotalLen) {
                    memmove(conn->GetRecvBuf(), conn->GetRecvCurBuf() + iCurrentPos, iTotalLen - iCurrentPos);
                }
                if (num == iRecvBufLeftSize) {
                    //也许还有数据没读取完毕
                    continue;
                } else {
                    break;
                }
            }
            else {
                //对端已经关闭连接
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
    } else {
        //TODO
    }
}

void CBaseServer::HandlePackage()
{
    auto it = m_ConnStat.begin();
    for (; it != m_ConnStat.end(); it++) {
        CConnState *pConnState = it->second;
        if (pConnState) {
            std::list<CPackage *> *recvList = pConnState->GetRecvPackList(); 
            while (!recvList->empty()) {
                CPackage *package = recvList->front();
                recvList->pop_front();
                this->OnRpcCall(package);
                delete package;
            }
        }
    }
}

int CBaseServer::OnRpcCall(CPackage *package)
{
    CMD_ID cmd_id;
    package->UnPackCmd(cmd_id); 
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;
    return RpcDispatch(cmd_id, package);
}

int CBaseServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    switch(cmd_id) {
        case MSG_CMD_RPC:
            return m_Rpc->Dispatch(package);
        default:
            break;
    }
    LOG_INFO("FromRpcCall end");
    return 0;
}

int CBaseServer::ConnectToServer(SERVER_TYPE server_type, SERVER_ID server_id, const char *ip, int port)
{
    int conn_fd = Connect(ip, port, 0);
    if (conn_fd < 0) {
        LOG_ERROR("accept new fd fail");
        return -1;
    }

    SetBlock(conn_fd, 0, NULL);

    AddFdToEpoll(conn_fd);

    //创建连接状态
    CConnState *conn = new CConnState(server_type, server_id, ip, port, CONN_MAILBOX_FLAG|CONN_HARBOR_FLAG);
    m_ConnStat.insert(std::make_pair(conn_fd, conn));

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

int CBaseServer::OnConnectFdCallBack(CConnState *conn)
{
    //如果是CENTERD，则发送注册信息
    if (conn->GetServerType() == SERVER_TYPE_CENTERD) {
        std::string pwd = m_Config->GetConfig("global", "CLUSTER_PWD");
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_SERVER_REGISTER);
        pack->PackInt(conn->GetServerType());
        pack->PackString(pwd);
        conn->PushSendPackList(pack);
    } else {
        //如果是CENTERD，则发送注册信息
        
    }
}

int CBaseServer::OnAcceptFdCallBack(CConnState *conn)
{
    
}

int CBaseServer::OnCloseFdCallBack(CConnState *conn)
{
    
}

}

