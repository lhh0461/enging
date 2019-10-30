#ifndef __DB_WORKER__
#define __DB_WORKER__

#include <string>
#include <list>
#include <iostream>
#include <cstdint>
#include <vector>

#include <mongocxx/client.hpp>

#include "WorkerMgr.h"

namespace XEngine
{

class CPackage;

class CDBWorker : public CWorker
{
public:
    CDBWorker();
    ~CDBWorker();
    int Init();
    int Process(void *Task);
private:
    int LoadDataFromDB(CPackage *package);
private:
    mongocxx::client *m_MongoConn;
};

}

#endif //__DB_WORKER__

