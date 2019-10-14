#include "GateServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "ConfigParser.h"
#include "Package.h"

namespace XEngine
{

CGateServer::CGateServer()
    :CBaseServer(SERVER_TYPE_GATED)
{
}

CGateServer::~CGateServer()
{

}
void CGateServer::Init()
{

}

void CGateServer::Run()
{

}

int CGateServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
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

