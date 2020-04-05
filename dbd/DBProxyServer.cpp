//dbd进程逻辑
/*
#include <mongocxx/instance.hpp>

#include "DBProxyServer.h"
#include "CLog.h"
#include "NetTool.h"
#include "Cmd.h"
#include "Common.h"
#include "CConnState.h"
#include "CConfigParser.h"
#include "WorkerMgr.h"
#include "CPackage.h"

namespace XEngine
{

CDBProxyServer::CDBProxyServer()
    :CBaseServer(SERVER_TYPE_DBD)
{
    int iWorkerNum = atoi(m_Config->GetConfig("global", "WORKER_NUM").c_str());
    m_WorkerMgr = new CWorkerMgr(iWorkerNum, DB_WORKER_TYPE);
}

CDBProxyServer::~CDBProxyServer()
{
    delete m_WorkerMgr;
}

void CDBProxyServer::Init()
{
    //TODO 调用到脚本层做初始化
    CBaseServer::Init(); 
    char cBuf[128] = {0};
    std::string strProjectName = m_Config->GetConfig("global", "PROJECT_NAME");
    m_DBName = sprintf(cBuf, "%s_%d", strProjectName.c_str(), GetClusterId());
}

void CDBProxyServer::Run()
{
    m_WorkerMgr->StartWorker();
    mongocxx::instance instance{};
    CBaseServer::Run();

}

//复写父类的接口
void CDBProxyServer::AddRecvPack(CPackage *package)
{
    CMD_ID cmd_id; 
    package->UnPackCmd(cmd_id);
    package->ResetUnpackOffset();
    if (IS_DBD_WORKER_CMD(cmd_id)) {
        //TODO 加到子线程的接收队列中
        m_WorkerMgr->PushTask(package);
    } else {
        m_RecvPackList.push_back(package);
    }
}

//复写父类的发送接口
void CDBProxyServer::SendPackage()
{
    std::list<CPackage *> send_list = m_WorkerMgr->GetSendList();
    if (send_list.size()  > 0) {
        while (!send_list.empty()) {
            CPackage *package = send_list.front();
            CConnState *conn = package->GetPkgConn();
            if (conn) {
                conn->PushSendPackList(package);
            }
        }
    }

    CBaseServer::SendPackage();
}


int CDBProxyServer::RpcDispatch(CMD_ID cmd_id, CPackage *package)
{
    //解析成功，直接返回
    int err_code = CBaseServer::RpcDispatch(cmd_id, package);
    if (err_code == ERR_UNKOWN_CMD) {
        switch(cmd_id) {
            default:
                LOG_ERROR("CDBProxyServer::RpcDispatch fail");
                break;
        }
        return ERR_UNKOWN_CMD;
    } else {
        return err_code;
    }
}

}

*/
