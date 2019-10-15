#ifndef __WORKER__
#define __WORKER__

#include <pthread.h>

namespace XEngine
{

typedef void *(*TASK_HANDLER_FUNC)(void *task);

class CWorkerMgr
{
public:
    CWorkerMgr(int worker_cnt, TASK_HANDLER_FUNC func, void *arg);
    ~CWorkerMgr();
    PushTask(void *task);
    StartWorker();
    StopWorker();
private:
    static int WorkerMain();
private:
    std::vector<pthread_t> m_WorkerIds;
    int m_WorkerCnt;
    bool m_Quit;
    pthread_cond_t m_TaskCond;
    pthread_mutex_t m_TaskMutex;
    std::list<void *> m_TaskList;
    TASK_HANDLER_FUNC m_TaskHandler;
    void *m_HandlerArg;
};

}

#endif //__WORKER__

