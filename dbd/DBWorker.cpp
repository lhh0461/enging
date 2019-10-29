//本文件在子线程执行
#include <string>
#include <list>
#include <iostream>
#include <cstdint>
#include <vector>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

#include "Package.h"

namespace XEngine
{

class CDBTask::CDBTask()
{
    //TODO 读取配置文件url
    std::string uri = m_Config->GetConfig("global", "MONGO_URI");
    m_MongoConn = new mongocxx::client(uri);
}

void CDBTask::OnLoadDataFromDB()
{
    int cluster_id = GetClusterId();
    
    std::string entity_name; 
    package->UnPackString(entity_name);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;
    package->UnPackString(pwd);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;

    std::string entity_name = GetEntityNameByType(entity_type);
    std::string collection;
    if (entity_name == "PlayerEntity") {
        collection = "user";
    } else {
        collection = "dat";
    }
    
}

int CDBTask::RpcDispatch(CPackage *package);
{
    switch(cmd_id) {
        case MSG_CMD_LOAD_DATA_FROM_DB:
            return OnLoadDataFromDB(package);
        default:
            LOG_ERROR("CDBProxyServer::RpcDispatch fail");
            break;
    }
    return ERR_UNKOWN_CMD;
}

}

