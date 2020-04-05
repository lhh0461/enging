#include "GateServer.h"
#include "CLog.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "CConfigParser.h"
#include "CPackage.h"

namespace XEngine
{

CGateServer::CGateServer()
    :CBaseServer(SERVER_TYPE_GATED)
{
}

CGateServer::~CGateServer()
{

}

int CGateServer::Init()
{

}

void CGateServer::Run()
{

}

int CGateServer::CmdDispatch(CMD_ID cmd_id, CPackage *package)
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

