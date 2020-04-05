#ifndef __GATE_SERVER__
#define __GATE_SERVER__

#include <unordered_map>

#include "CPackage.h"
#include "CRpc.h"
#include "CConfigParser.h"
#include "CBaseServer.h"
#include "CConnState.h"

namespace XEngine
{

class CGateServer : public CBaseServer
{
public:
    CGateServer();
    ~CGateServer();
    int Init();
    void Run();
    int CmdDispatch(CMD_ID cmd_id, CPackage *package);
private:
    //CLoadBalance *m_LoadBalance;
    //CLoadBalance *m_LoadBalance;
};

}

#endif //__GATE_SERVER__

