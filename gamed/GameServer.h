#ifndef __GAME_SERVER__
#define __GAME_SERVER__

#include <unordered_map>

#include "CPackage.h"
#include "CRpc.h"
#include "CConfigParser.h"
#include "CBaseServer.h"
#include "CConnState.h"
#include "EntityMgr.h"

namespace XEngine
{

class CGameServer : public CBaseServer
{
public:
    CGameServer();
    ~CGameServer();
    int Init();
    int CmdDispatch(CMD_ID cmd_id, CPackage *package);
private:
    CEntityMgr *m_EntityMgr;
};

}

#endif //__GAME_SERVER__

