#ifndef __GAME_SERVER__
#define __GAME_SERVER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"
#include "EntityMgr.h"

namespace XEngine
{

class CGameServer : public CBaseServer
{
public:
    CGameServer();
    ~CGameServer();
    void Init();
    int RpcDispatch(CMD_ID cmd_id, CPackage *package);
private:
    CEntityMgr *m_EntityMgr;
};

}

#endif //__GAME_SERVER__

