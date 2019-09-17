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
        if (it != m_ConnStat.end()) {
            CConnState *conn = it->second;
            if (conn != NULL) {
                char *pRecvPosBuf = conn->GetRecvPosBuf();
                int iRecvBufLeftSize = conn->GetRecvBufLeftSize();
                size_t num = ::recv(fd, pRecvPosBuf, iRecvBufLeftSize, 0);
                if (num == (size_t)-1) {
                    if (errno == EINTR) {
                        continue;
                    }
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    }
                    cout << "errno=" << errno << ",errmsg="  << strerror(errno)  << endl;
                    break;
                }
                else if (num > 0) {
                    size_t totallen = conn->SetRecvBufLen(conn->GetRecvBufPos() + num);
                    if (totallen < CPackage::PKG_HEADER_SIZE) {
                         //包头还没读取够
                         break;
                    }
                    //包还没读取够
                    size_t packsize = *(uint64_t *)conn->GetRecvPosBuf();
                    if (totallen < packsize) {
                         break;
                    }
                    
                    CPackage *package = new CPackage(conn->GetRecvPosBuf() + sizeof(uint64_t), packsize);
                    conn->GetRecvPackList().push_back(package);

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
        }
    }
}

void CBaseServer::HandlePackage()
{
    auto it = m_ConnStat.begin();
    for (; it != m_ConnStat.end(); it++) {
        std::list<CPackage *> recvList = it->second->GetRecvPackList(); 
        if (!recvList.empty()) {
            auto it2 = recvList.begin();
            for (; it2 != recvList.end(); it2++) {
                this->FromRpcCall(*it2);
            }
        }
    }
}

int CBaseServer::FromRpcCall(CPackage *package)
{
    uint16_t cmd_id = package->GetCmd(); 
    switch(cmd_id) {
        case MSG_CMD_RPC:
            m_Rpc->Dispatch(package);
            break;
        default:
            break;
    }
}

}

