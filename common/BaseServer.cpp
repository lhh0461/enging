#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "BaseServer.h"
#include "Log.h"
#include "ConfigParser.h"

using namespace std;

CBaseServer::CBaseServer()
    :m_epoll_fd(0), m_listen_fd(0), m_Config(NULL)
{
    m_Config = new CConfigParser();
}

CBaseServer::~CBaseServer()
{

}

void CBaseServer::Init()
{
    if (m_Config->Parser("./config.ini") == false) {
        LOG_ERROR("CBaseServer::Init desc=Init Fail");
        exit(1);
    }

    string ip = m_Config->GetConig("global", "IP");
    int port = m_Config->GetConig("global", "PORT");

    m_epoll_fd = epoll_create(MAX_EVENT); 
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口
    bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(fd, 1024);

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
                else if (events[i].events & EPOLLOUT) {
                    HandleWriteMsg(events[i].data.fd);
                }
            }
        }
    }
}

void CBaseServer::HandleNewConnection()
{
    struct sockaddr addr;
    socklen_t addrlen;
    int conn_fd = accept(m_listen_fd, (struct sockaddr *) &addr, &addrlen);

    int flag = fcntl(conn_fd, F_GETFL, 0); 
    fcntl(conn_fd, F_SETFL, flag | O_NONBLOCK);

    AddFdToEpoll(conn_fd);

    CClientCtx *client = new CClientCtx();
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
            CConnCtx *ctx = it->second;
            CBuffer *pRecvBuf = ctx->GetRecvBuf();
            std::list<CPackage *> recvList = ctx->GetRecvPackList(); 
            if (ctx != NULL) {
                size_t left = pRecvBuf->GetBuffSize() - pRecvBuf->GetDataLen();
                size_t num = ::recv(fd, pRecvBuf->GetBuff(), left, 0);
                if (num == -1) {
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
                    size_t totallen = pRecvBuf->SetDataLen(pRecvBuf->GetDataLen() + num);
                    if (totallen < HEAD_SIZE) {
                         //包头还没读取够
                         break;
                    }
                    size_t packsize = *(uint64_t *)pRecvBuf->GetBuff();
                    if (totallen < packsize) {
                         break;
                    }
                    
                    CPackage *package = new CPackage(pRecvBuf->GetBuff() + sizeof(uint64_t), totallen)
                    recvList.push_back(package);

                    if (num == left) {
                        //TODO 也许还有数据没读取完毕
                        continue;
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

void CBaseServer::HandlePackage()
{
    auto it = m_ConnStat.begin();
    for (; it != m_ConnStat.end() it++) {
        std::list<CPackage *> recvList = it->second->GetRecvPackList(); 
        if (!recvList.empty()) {
            auto it2 = recvList.begin();
            for (; it2 != recvList.end() it2++) {
                this->FromRpcCall(*it2);
            }
        }
    }
}

int CBaseServer::FromRpcCall(const CPackage *package)
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
