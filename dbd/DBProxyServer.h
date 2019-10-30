#ifndef __DB_PROXY_SERVER__
#define __DB_PROXY_SERVER__

#include <string>

#include "BaseServer.h"
#include "Common.h"

namespace XEngine
{

class CWorkerMgr;
class CPackage;

class CDBProxyServer : public CBaseServer
{
public:
    CDBProxyServer();
    ~CDBProxyServer();
    void Init();
    void Run();
    void AddRecvPack(CPackage *package);
    void SendPackage();
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
    std::string GetDBName() { return m_DBName; };
private:
    CWorkerMgr *m_WorkerMgr;
    std::string m_DBName;
};

}

#endif //__DB_PROXY_SERVER__

