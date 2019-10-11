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
protected:
    virtual int OnAcceptFdCallBack(CConnState *conn);
    virtual int OnConnectFdCallBack(CConnState *conn);
    virtual int OnCloseFdCallBack(CConnState *conn);
private:
    void HandleNewConnection();
    void AddFdToEpoll(int fd);
    void HandleRecvMsg(int fd);
    void HandlePackage();
    int ConnectToServer(SERVER_ID server_id, const char *ip, int port);
    CConnState *GetServerConnById(SERVER_ID server_id);
    CConnState *AddServerConn(SERVER_ID server_id);
    CConnState *DelServerConn(SERVER_ID server_id);
private:
    int m_ServerId; 
    int m_epoll_fd; 
    int m_listen_fd; 
    int m_tick;
    SERVER_TYPE m_ServerType; 
    CRpc *m_Rpc;
    CConfigParser *m_Config;
    std::unordered_map<int, CConnState *> m_ConnStat;
    std::unordered_map<SERVER_ID, CConnState *> m_ServerId2Conn;
};

}

#endif //__BASE_SERVER__

