#ifndef __BASE_SERVER__
#define __BASE_SERVER__

#include <unordered_map>
#include "Package.h"


class CBaseServer
{
public:
    enum
    {
        MAX_EVENT = 9999,
    };
public:
    CBaseServer();
    ~CBaseServer();
    virtual ~CBaseServer();
    virtual void Init();
    virtual void Run();
protected:
    virtual int FromRpcCall(const CPackage *package);
private:
    void HandleNewConnection();
    void AddFdToEpoll(int fd);
    void HandleRecvMsg(int fd);
    void HandlePackage();
private:
    int m_epoll_fd; 
    int m_listen_fd; 
    int m_vfd; 
    int m_tick; 
    CRpc *m_Rpc;
    CConfigParser *m_Config;
    std::unordered_map<int, CClientCtx *> m_ConnStat;
};

#endif //__BASE_SERVER__

