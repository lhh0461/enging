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
    SetLogFileName(m_Config->GetConfig("centerd", "LOG_PATH").c_str());
}

CCenterServer::~CCenterServer()
{

}

void CCenterServer::Init()
{
    CBaseServer::Init();

    std::string ip = m_Config->GetConfig("centerd", "IP");
    int port = atoi(m_Config->GetConfig("centerd", "PORT").c_str());

    AddListenFd(ip, port);
}

void CCenterServer::Run()
{
    CBaseServer::Run();
}

//每个集群
//高7位表示集群ID从1~2^7-1 (100台服足够)
//低9位表示集群内的服务器标识从1~2^9-1
//其中前3位表示服务器的大类型标识，6为表示服务器的ID所以最多是64台服，单集群内
//第4位可能表示服务器的子类型（某些服务器需要4位来表示服务器的类型，那么就最多有32台）
SERVER_ID CCenterServer::AssignServerId(SERVER_TYPE iServerType)
{
    CLUSTER_ID iClusterId = GetClusterId();
    SERVER_ID iServerId = 0;
    auto it = m_ServerCount.find(iServerType);
    if (it == m_ServerCount.end()) {
        m_ServerCount.insert(std::make_pair(iServerType, 1));
        it = m_ServerCount.find(iServerType);
    }
    iServerId = iClusterId << 9 | iServerType << 6 | it->second++;
    return iServerId;
}

int CCenterServer::AssignServerPort()
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

    std::string mypwd = m_Config->GetConfig("global", "CLUSTER_PWD");
    if (pwd != mypwd) {
        LOG_ERROR("pwd is not match,mypwd=%s,pwd=%s", mypwd.c_str(), pwd.c_str());
        return ERR_FAIL;
    }

    SERVER_ID server_id = AssignServerId(server_type);
    int port = AssignServerPort();
    LOG_DEBUG("OnServerRegister server_type=%d,pwd=%s", server_type, pwd.c_str());
    
    CConnState *conn = package->GetPkgConn();
    if (conn) {
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_SERVER_REGISTER);
        pack->PackInt(conn->GetServerType());
        pack->PackString(pwd);
        conn->PushSendPackList(pack);
    }

    return ERR_SUCCESS;
}

int CCenterServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    LOG_DEBUG("rpc dispatch cmd_id=%d", cmd_id);
    int err_code = CBaseServer::RpcDispatch(cmd_id, package);
    if (err_code == ERR_UNKOWN_CMD) {
        switch(cmd_id) {
            case MSG_CMD_SERVER_REGISTER:
                return OnServerRegister(package);
            default:
                break;
        }
    } else {
        return err_code;
    }
}

}

