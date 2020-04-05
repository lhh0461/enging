#include "GameServer.h"
#include "CLog.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "CConfigParser.h"
#include "CPackage.h"

namespace XEngine
{

CGameServer::CGameServer()
    :CBaseServer(SERVER_TYPE_GAMED)
{
    SetLogFileName(m_Config->GetConfig("gamed", "LOG_PATH").c_str());
}

CGameServer::~CGameServer()
{

}
int CGameServer::Init()
{
    std::cout << m_Config->GetConfig("gamed", "LOG_PATH") << std::endl;
    CBaseServer::Init();
}

int CGameServer::CmdDispatch(CMD_ID cmd_id, CPackage *package)
{
    //解析成功，直接返回
    if (CBaseServer::CmdDispatch(cmd_id, package)) {
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

