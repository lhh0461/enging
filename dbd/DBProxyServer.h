#ifndef __DB_PROXY_SERVER__
#define __DB_PROXY_SERVER__

#include <unordered_map>

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
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
private:
    CWorkerMgr *m_WorkerMgr;
};

}

#endif //__DB_PROXY_SERVER__

