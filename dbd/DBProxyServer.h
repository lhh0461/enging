#ifndef __DB_PROXY_SERVER__
#define __DB_PROXY_SERVER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "Worker.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"

namespace XEngine
{

class CWorkerMgr;

class CDBProxyServer : public CBaseServer
{
public:
    CDBProxyServer();
    ~CDBProxyServer();
    void Init();
    void Run();
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
    mongocxx::instance & GetMongoInstance();
protected:
private:
private:
    mongocxx::instance instance;
    CWorkerMgr *m_WorkerMgr;
};

}

#endif //__DB_PROXY_SERVER__

