#include "GameServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "ConfigParser.h"
#include "Package.h"

namespace XEngine
{

CGameServer::CGameServer()
    :CBaseServer(SERVER_TYPE_GAMED)
{
}

CGameServer::~CGameServer()
{

}
void CGameServer::Init()
{
    SetLogFileName(m_Config->GetConfig("gamed", "LOG_PATH").c_str());
    CBaseServer::Init();
}

int CGameServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    //解析成功，直接返回
    if (CBaseServer::RpcDispatch(cmd_id, package)) {
        return 1;
    }
    
    switch(cmd_id) {
        default:
            //TODO error log
            break;
    }
    return ERR_UNKOWN_CMD;
}

}

