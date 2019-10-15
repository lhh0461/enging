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
    m_WorkerMgr = new CWorkerMgr();
}

CDBProxyServer::~CDBProxyServer()
{
    delete m_WorkerMgr;
}

void CDBProxyServer::Init()
{
    //TODO 调用到脚本层做初始化
    m_WorkerMgr->Init();
}

void CDBProxyServer::Run()
{
    m_WorkerMgr->StartWorker();
    //TODO
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
        m_Worker->PushPackage(package);
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
            CConnState *conn = package->GetPackConn();
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
            case MSG_CMD_LOAD_DATA_FROM_DB:
                return OnLoadDataFromDB(package);
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

