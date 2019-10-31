//本文件在子线程执行
#include <string>
#include <list>
#include <iostream>
#include <cstdint>
#include <vector>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "Rpc.h"
#include "Package.h"
#include "Cmd.h"
#include "ConfigParser.h"
#include "DBProxyServer.h"
#include "DBWorker.h"

namespace XEngine
{

CDBWorker::CDBWorker()
    :CWorker()
{
    CConfigParser *conf = g_Server->GetConfig();
    std::string uri_cfg = conf->GetConfig("global", "MONGO_URI");
    mongocxx::uri uri(uri_cfg);
    m_MongoClient = new mongocxx::client(uri);
}

int CDBWorker::Init()
{
}

int CDBWorker::LoadDataFromDB(CPackage *package)
{
    std::string entity_name; 
    package->UnPackString(entity_name);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;

    string key;
    package->UnPackString(key);
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;

    std::string coll_name;
    if (entity_name == "PlayerEntity") {
        coll_name = "user";
    } else {
        coll_name = "dat";
    }

    CDBProxyServer *pServer = (CDBProxyServer *)g_Server;
    CLUSTER_ID cluster_id = pServer->GetClusterId();
    auto db = m_MongoClient->database(pServer->GetDBName().c_str());

    auto collection = db[coll_name];
    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

int CDBWorker::Process(void *Task)
{
    CPackage *package = (CPackage *)Task;
    CMD_ID cmd_id;
    package->UnPackCmd(cmd_id); 
    if (package->GetErrCode() > 0) return ERR_UNPACK_FAIL;
    switch(cmd_id) {
        case MSG_CMD_LOAD_DATA_FROM_DB:
            return LoadDataFromDB(package);
        default:
            LOG_ERROR("CDBWorker::Process fail");
            break;
    }
    return ERR_UNKOWN_CMD;
}

}

