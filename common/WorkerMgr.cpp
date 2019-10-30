#include "WorkerMgr.h"

namespace XEngine
{

CWorker::CWorker()
    :m_WorkerMgr(NULL),m_UserData(NULL)
{
    pthread_mutex_init(&m_Mutex, NULL);
}

CWorker::~CWorker()
{ 
    pthread_mutex_destroy(&m_Mutex);
};

int CWorker::ProduceTask(void *pTask)
{
    pthread_mutex_lock(&m_Mutex);
    m_TaskQueue.push_back(pTask);
    pthread_mutex_unlock(&m_Mutex);
}

CWorkerMgr::CWorkerMgr(int iWorkerNum, eWorkerType iWorkerType)
    :m_WorkerType(iWorkerType),m_WorkerCnt(iWorkerNum),m_Counter(0),m_Quit(0)
{
    m_Workers.reserve(iWorkerNum);
    for (int i = 0; i < m_WorkerCnt; i++) {
        CWorker* pWorker = CreateWorkerFactory(iWorkerType);
        pWorker->m_WorkerMgr = this;
        m_Workers[i] = pWorker;
    }
}

CWorkerMgr::~CWorkerMgr()
{
    for (int i = 0; i < m_WorkerCnt; i++) {
        if (m_Workers[i]) {
            delete m_Workers[i];
        }
    }
    m_Workers.clear();
}

int CWorkerMgr::StartWorker()
{
    pthread_t pid;
    int ret;
    for (int i = 0; i < m_WorkerCnt; i++) {
        CWorker *pWorker = m_Workers[i];
        ret = pthread_create(&pWorker->m_Id, NULL, WorkerMain, pWorker);
        if (ret != 0) {
            return -1;
        }
    }
    //TODO join thread
    return 0;
}

int CWorkerMgr::StopWorker()
{
    m_Quit = 1;
    return 0;
}

void *CWorkerMgr::WorkerMain(void *arg)
{
    CWorker *pWorker = (CWorker *)arg;
    CWorkerMgr *pWorkerMgr = pWorker->m_WorkerMgr;
    while (!pWorkerMgr->m_Quit)
    {
        void *pTask = NULL;
        pthread_mutex_lock(&pWorker->m_Mutex);
        if (!pWorker->m_TaskQueue.empty()) {
            pTask = pWorker->m_TaskQueue.front();
        }
        pthread_mutex_unlock(&pWorker->m_Mutex);
        if (pTask) {
            pWorker->Process(pTask);
        }
    }
    return 0;
}

int CWorkerMgr::PushTask(void *pTask)
{
    CWorker *pWorker = m_Workers[++m_Counter % m_WorkerCnt];
    pWorker->ProduceTask(pTask);
    return 0;
}

}
