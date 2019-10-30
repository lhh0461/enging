#ifndef __WORKER_FACTORY__
#define __WORKER_FACTORY__

namespace XEngine
{
class CWorker;

enum eWorkerType
{
    DB_WORKER_TYPE = 1,
    HTTP_WORKER_TYPE = 2,
};

CWorker *CreateWorkerFactory(eWorkerType type);
}

#endif //__WORKER_FACTORY__

