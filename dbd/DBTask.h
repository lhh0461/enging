#ifndef __DBTASK__
#define __DBTASK__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "Worker.h"
#include "ConfigParser.h"
#include "BaseServer.h"
#include "ConnState.h"

namespace XEngine
{

class CDBTask
{
public:
    CDBTask();
    ~CDBTask();
    int RpcDispatch(CPackage *package);
protected:
private:
    void OnLoadDataFromDB();
private:
    mongocxx::client *m_MongoConn;
};

}

#endif //__DBTASK__

