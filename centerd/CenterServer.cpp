#include "CenterServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "ConfigParser.h"
#include "Package.h"

namespace XEngine
{

CCenterServer::CCenterServer()
    :CBaseServer(SERVER_TYPE_CENTERD)
{
}

CCenterServer::~CCenterServer()
{

}
void CCenterServer::Init()
{

}

void CCenterServer::Run()
{

}

int CCenterServer::OnServerRegister(CPackage *package)
{
    SERVER_TYPE server_type; 
    std::string pwd; 

    package->UnPackInt(server_type);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;
    package->UnPackString(pwd);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;

    //TODO

    return ERR_SUCCESS;
}

int CCenterServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    //解析成功，直接返回
    if (CBaseServer::RpcDispatch(cmd_id, package)) {
        return 1;
    }
    
    switch(cmd_id) {
        case MSG_CMD_SERVER_REGISTER:
            return OnServerRegister(package);
        default:
            //TODO error log
            break;
    }
    return ERR_UNKOWN_CMD;
}

}

