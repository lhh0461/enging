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
    virtual int Init();
    virtual void Run();
protected:
    virtual int CmdDispatch(CMD_ID cmd, CPackage *package); //子类覆盖
    virtual void AddRecvPack(CPackage *package) { m_RecvPackList.push_back(package); };
    virtual void SendPackage();
    void AddListenFd(std::string ip, int port);
protected:
    virtual int OnAcceptFdCallBack(CConnState *conn);
    virtual int OnConnectFdCallBack(CConnState *conn);
    virtual int OnCloseFdCallBack(CConnState *conn);
public:
    int ConnectToServer(SERVER_TYPE server_type, SERVER_ID server_id, const char *ip, int port);
    CConnState *GetServerConnById(SERVER_ID server_id);
    int GetServerConnByType(SERVER_TYPE server_type, std::list<CConnState *> &conn_list);
    CLUSTER_ID GetClusterId() { return m_ClusterId; };
    SERVER_ID GetServerId() { return m_ServerId; };
    SERVER_TYPE GetServerType() { return m_ServerType; };
    const CConfigParser *GetConfig() { return m_Config; };
private:
    int OnRpcCall(CPackage *package);
    int HandleNewConnection();
    void AddFdToEpoll(int fd, uint32_t events);
    void ModifFdEpollEvents(int fd, uint32_t events);
    void HandleRecvMsg(CConnState *conn);
    void HandleWriteMsg(CConnState *conn);
    void HandlePackage();
    CConnState *AddServerConn(SERVER_ID server_id);
    CConnState *DelServerConn(SERVER_ID server_id);
protected:
    SERVER_ID m_ServerId; //中心服分配的具体ID
    SERVER_TYPE m_ServerType; //服务器类型
    CLUSTER_ID m_ClusterId; //集群ID配置决定
    CConfigParser *m_Config;
    int m_EpollFd; 
    int m_ListenFd; 
    CRpc *m_Rpc;
    std::list<CPackage *> m_RecvPackList;
    std::unordered_map<int, CConnState *> m_ConnStat;
    std::unordered_map<SERVER_ID, CConnState *> m_ServerId2Conn;
};

extern CBaseServer *g_Server;

}

#endif //__BASE_SERVER__

