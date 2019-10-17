#include "WorkerMgr.h"
#include "Mutex.h"

namespace XEngine
{

/*
CWorkerMgr::CWorkerMgr(int worker_cnt, TASK_HANDLER_FUNC func)
    :m_WorkerCnt(worker_cnt), m_TaskHandler(func)
{
    pthread_mutex_init(&m_Mutex, NULL);
}

CWorkerMgr::~CWorkerMgr()
{
    pthread_mutex_destroy(&m_Mutex);
}

int CWorkerMgr::StartWorker()
{
    pthread_t pid;
    int ret;
    m_WorkerIds.reserve(m_WorkerCnt);
    void *ptr = this;
    for (int i = 0; i < m_WorkerCnt; i++) {
        ret = pthread_create(&pid, NULL, WorkerMain, ptr);
        if (ret != 0) {
            return -1;
        }
        m_WorkerIds[0] = pid;
    }
    return 0;
}

int CWorkerMgr::StopWorker()
{
    m_Quit = 1;
    return 0;
}

void *CWorkerMgr::WorkerMain(void *arg)
{
    CWorkerMgr *mgr = (CWorkerMgr *)arg;
    while (!m_Quit)
    {
        pthread_mutex_lock(&m_Mutex);
        if (m_TaskList.empty()) {
            rv = pthread_cond_timewait(&m_TaskCond, &m_Mutex, 50);
            if (rv != 0) {
                pthread_mutex_unlock(&m_Mutex);
                if (rv != ETIMEDOUT) {
                    errno = rv;
                }
                return rv;
            }
        }
        void *task = m_TaskList.front();
        pthread_mutex_unlock(&m_Mutex);
        m_TaskHandler(m_Arg, task);
    }
    return 0;
}

int CWorkerMgr::PushTask(void *task)
{
    pthread_mutex_lock(&m_Mutex);
    m_TaskList.push_back(task);
    pthread_mutex_unlock(&m_Mutex);
    return 0;
}
*/
