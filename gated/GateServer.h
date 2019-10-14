#ifndef __GATE_SERVER__
#define __GATE_SERVER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"

namespace XEngine
{

class CGateServer : public CBaseServer
{
public:
    CGateServer();
    ~CGateServer();
    void Init();
    void Run();
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
private:
    //CLoadBalance *m_LoadBalance;
    //CLoadBalance *m_LoadBalance;
};

}

#endif //__GATE_SERVER__

