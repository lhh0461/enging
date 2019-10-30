#include "../dbd/DBWorker.h"
#include "WorkerFactory.h"

namespace XEngine
{

CWorker *CreateWorkerFactory(eWorkerType type)
{
    if (type == DB_WORKER_TYPE) {
        return new CDBWorker();
    }
}

}
