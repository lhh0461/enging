#include <sys/epoll.h>
//#include <string>
#include <iostream>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "BaseServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "ConfigParser.h"
#include "Package.h"

using namespace std;

namespace XEngine
{

CBaseServer::CBaseServer():
    m_epoll_fd(0), m_listen_fd(0)
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

    m_epoll_fd = epoll_create(MAX_EVENT); 
    
    int fd = Listen(ip.c_str(), port, 1024);
    if (fd < 0) {
        LOG_ERROR("listen port fail;port=%d", port);
        exit(1);
    }

    LOG_INFO("server init success");

    m_listen_fd = fd;
    AddFdToEpoll(fd);

    Py_Initialize();
}

void CBaseServer::Run()
{
    struct epoll_event events[MAX_EVENT];
    for (;;)
    {
        int nfds = epoll_wait(m_epoll_fd, events, MAX_EVENT, 500);
        for(int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == m_listen_fd) {
                HandleNewConnection();
            }
            else { 
                if (events[i].events & EPOLLIN) {
                    HandleRecvMsg(events[i].data.fd);
                }
                //else if (events[i].events & EPOLLOUT) {
                //    HandleWriteMsg(events[i].data.fd);
                //}
            }
        }
        this->HandlePackage();
    }
}

void CBaseServer::HandleNewConnection()
{
    struct sockaddr_in addr;
    socklen_t addrlen;
    int conn_fd = accept(m_listen_fd, (struct sockaddr *) &addr, &addrlen);
    if (conn_fd < 0) {
        LOG_ERROR("accept new fd fail");
        return;
    }

    SetBlock(conn_fd, 0, NULL);

    AddFdToEpoll(conn_fd);

    const char *ip = inet_ntoa(addr.sin_addr);
    int port = addr.sin_port;

    CConnState *client = new CConnState(ip, port, CConnState::CONN_CLIENT_FLAG);
    m_ConnStat.insert(std::make_pair(conn_fd, client));
}

void CBaseServer::AddFdToEpoll(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

void CBaseServer::HandleRecvMsg(int fd)
{
    for (;;)
    {
        auto it = m_ConnStat.find(fd);
        if (it == m_ConnStat.end()) {
            LOG_ERROR("cant find fd conn stat");
            continue; 
        }
        CConnState *conn = it->second;
        if (conn != NULL) {
            //剩余的的缓冲区
            char *pRecvCurBuf = conn->GetRecvCurBuf();
            int iRecvBufLeftSize = conn->GetRecvBufLeftSize();
            //读取数据到缓冲区
            size_t num = ::recv(fd, pRecvCurBuf, iRecvBufLeftSize, 0);
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
            }
        }
        break;
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
                this->FromRpcCall(package);
                delete package;
            }
        }
    }
}

int CBaseServer::FromRpcCall(CPackage *package)
{
    uint16_t cmd_id;
    package->UnPackCmd(cmd_id); 
    
    switch(cmd_id) {
        case MSG_CMD_RPC:
            m_Rpc->Dispatch(package);
            break;
        default:
            break;
    }
    LOG_INFO("FromRpcCall end");
    return 0;
}

}

