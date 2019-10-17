#ifndef __WORKER_MGR__
#define __WORKER_MGR__

#include <pthread.h>
#include <list>
#include <vector>

namespace XEngine
{

typedef void *(*TASK_HANDLER_FUNC)(void *task);

class CWorkerMgr
{
public:
    CWorkerMgr(int worker_cnt, TASK_HANDLER_FUNC func);
    ~CWorkerMgr();
    int PushTask(void *task);
    int StartWorker();
    int StopWorker();
    static void *WorkerMain(void *);
private:
    std::vector<pthread_t> m_WorkerIds;
    int m_WorkerCnt;
    bool m_Quit;
    pthread_cond_t m_TaskCond;
    pthread_mutex_t m_Mutex;
    std::list<void *> m_TaskList;
    TASK_HANDLER_FUNC m_TaskHandler;
};

}

#endif //__WORKER_MGR__

