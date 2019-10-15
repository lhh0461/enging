#include "Worker.h"

namespace XEngine
{

CWorkerMgr::CWorkerMgr(int worker_cnt, void *(*func)(void *), void *arg)
    :m_WorkerCnt(worker_cnt), m_PackHandler(func), m_Arg(arg)
{
}

CWorkerMgr::~CWorkerMgr()
{
    pthread_mutex_destroy(&m_Mutex);
}

int CWorkerMgr::StartWorker()
{
    pthread_mutex_init(&m_Mutex, NULL);
    pthread_t pid;
    for (i = 0; i < m_WorkerCnt; i++) {
        ret = pthread_create(&pid, NULL, WorkerMain, NULL);  
        if (ret != 0)  
        {  
            perror("pthread_1_create");  
        }
        
    }
    return 0;
}

int CWorkerMgr::StopWorker()
{
    b_Quit = 1;
    return 0;
}

int CWorkerMgr::WorkerMain()
{
    while (!b_Quit)
    {
        pthread_mutex_lock(&m_TaskMutex);
        if (m_TaskList.empty()) {
            rv = pthread_cond_timewait(&m_TaskCond, &m_TaskMutex, 50);
            if (rv != 0) {
                pthread_mutex_unlock(&m_TaskMutex);
                if (rv != ETIMEDOUT) {
                    errno = rv;
                    log_perror("E00034:\tpthread_cond_timedwait() failed when try to get_work_fd()");
                }
                return rv;
            }
        }
        void *task = m_TaskList.front();
        pthread_mutex_unlock(&m_TaskMutex);
        m_TaskHandler(m_Arg, task);
    }
    return 0;
}

int CWorker::PushTask(void *task)
{
    pthread_mutex_lock(&m_TaskMutex);
    m_TaskList.push_back(task);
    pthread_mutex_unlock(&m_TaskMutex);
    return 0;
}
