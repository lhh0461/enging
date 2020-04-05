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

