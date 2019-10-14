#ifndef __BASE_SERVER__
#define __BASE_SERVER__

#include <list>
#include <unordered_map>
#include "Common.h"

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
    CBaseServer(SERVER_TYPE server_type);
    virtual ~CBaseServer();
    virtual void Init();
    virtual void Run();
protected:
    int RpcDispatch(CMD_ID cmd, CPackage *package); //子类覆盖
protected:
    virtual int OnAcceptFdCallBack(CConnState *conn);
    virtual int OnConnectFdCallBack(CConnState *conn);
    virtual int OnCloseFdCallBack(CConnState *conn);
public:
    int ConnectToServer(SERVER_TYPE server_type, SERVER_ID server_id, const char *ip, int port);
    CConnState *GetServerConnById(SERVER_ID server_id);
    int GetServerConnByType(SERVER_TYPE server_type, std::list<CConnState *> &conn_list);
    CEntityMgr *GetEntityMgr() { return m_EntityMgr; };
private:
    int OnRpcCall(CPackage *package);
    void HandleNewConnection();
    void AddFdToEpoll(int fd);
    void HandleRecvMsg(CConnState *conn);
    void HandleWriteMsg(CConnState *conn);
    void HandlePackage();
    CConnState *AddServerConn(SERVER_ID server_id);
    CConnState *DelServerConn(SERVER_ID server_id);
protected:
    SERVER_ID m_ServerId; 
    SERVER_TYPE m_ServerType; 
    int m_EpollFd; 
    int m_ListenFd; 
    CRpc *m_Rpc;
    CConfigParser *m_Config;
    std::unordered_map<int, CConnState *> m_ConnStat;
    std::unordered_map<SERVER_ID, CConnState *> m_ServerId2Conn;
    CEntityMgr *m_EntityMgr;
};

extern CBaseServer *g_Server;

}

#endif //__BASE_SERVER__

