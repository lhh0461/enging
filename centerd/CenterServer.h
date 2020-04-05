#ifndef __CENTER_SERVER__
#define __CENTER_SERVER__

#include <unordered_map>

#include "CPackage.h"
#include "CRpc.h"
#include "CConfigParser.h"
#include "CBaseServer.h"
#include "CConnState.h"

namespace XEngine
{

class CCenterServer : public CBaseServer
{
public:
    CCenterServer();
    ~CCenterServer();
    int Init();
    void Run();
    int CmdDispatch(CMD_ID cmd_id, CPackage *package);
protected:
    int FromRpcCall(CPackage *package);
private:
    int OnServerRegister(CPackage *package);
    SERVER_ID AssignServerId(SERVER_TYPE iServerType);
    int AssignServerPort();
private:
    //CLoadBalance *m_LoadBalance;
    std::unordered_map<SERVER_TYPE, int> m_ServerCount;
};

}

#endif //__CENTER_SERVER__

