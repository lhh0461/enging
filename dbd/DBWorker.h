#ifndef __DB_WORKER__
#define __DB_WORKER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "Worker.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"

namespace XEngine
{

class CDBWorker
{
public:
    CDBWorker();
    ~CDBWorker();
    int Init();
    int Process(void *package);
private:
    void OnLoadDataFromDB();
private:
    mongocxx::client *m_MongoConn;
    static mongocxx::instance *m_MongoInstance;
};

}

#endif //__DB_WORKER__

