#include "DBProxyServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "ConfigParser.h"
#include "Package.h"

namespace XEngine
{

CDBProxyServer::CDBProxyServer()
    :CBaseServer(SERVER_TYPE_DBD)
{
}

CDBProxyServer::~CDBProxyServer()
{

}

void CDBProxyServer::Init()
{

}

void CDBProxyServer::Run()
{

}

void CDBProxyServer::OnLoadDataFromDB()
{
    
}

int CDBProxyServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    //解析成功，直接返回
    if (CBaseServer::RpcDispatch(cmd_id, package)) {
        return 1;
    }
    
    switch(cmd_id) {
        case MSG_CMD_LOAD_DATA_FROM_DB:
            return OnLoadDataFromDB(package);
        default:
            //TODO error log
            break;
    }
    return ERR_UNKOWN_CMD;
}

}

