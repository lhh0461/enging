#include "CenterServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "ConfigParser.h"
#include "Package.h"

using namespace std;

namespace XEngine
{

CCenterServer::CCenterServer()
{
}

CCenterServer::~CCenterServer()
{

}

int CCenterServer::OnServerRegister(CPackage *package)
{
    return 1;
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
            break;
    }
    return 0;
}

}

