#ifndef __DB_PROXY_SERVER__
#define __DB_PROXY_SERVER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"

namespace XEngine
{

class CDBProxyServer : public CBaseServer
{
public:
    CDBProxyServer();
    ~CDBProxyServer();
    void Init();
    void Run();
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
protected:
    int FromRpcCall(CPackage *package);
private:
    int OnServerRegister(CPackage *package);
private:
};

}

#endif //__DB_PROXY_SERVER__

