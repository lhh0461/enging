#ifndef __BASE_SERVER__
#define __BASE_SERVER__

#include <list>
#include <unordered_map>

namespace XEngine
{

class CRpc;
class CPackage;
class CConnState;
class CConfigParser;

class CBaseServer
{
public:
    enum
    {
        MAX_EVENT = 9999,
    };
public:
    CBaseServer();
    virtual ~CBaseServer();
    virtual void Init();
    virtual void Run();
protected:
    virtual int FromRpcCall(CPackage *package);
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
    std::unordered_map<int, CConnState *> m_ConnStat;
    std::unordered_map<SERVER_TYPE, std::list<int>> m_ServerType2Fd;
};

}

#endif //__BASE_SERVER__

