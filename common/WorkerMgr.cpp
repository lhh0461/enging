/*
#include "WorkerMgr.h"

namespace XEngine
{

CWorker::CWorker()
    :m_WorkerMgr(NULL),m_UserData(NULL)
{
}

CWorker::~CWorker()
{ 
};

int CWorker::PushMsg(void *pTask)
{
    pthread_mutex_lock(&m_Mutex);
    m_TaskQueue.push_back(pTask);
    pthread_mutex_unlock(&m_Mutex);
}


//------------------------------------------------------------

CWorkerMgr::CWorkerMgr(int iWorkerNum, eWorkerType iWorkerType)
    :m_WorkerType(iWorkerType),m_WorkerCnt(iWorkerNum),m_Counter(0),m_Quit(0)
{
    m_WorkerList.reserve(iWorkerNum);
    for (int i = 0; i < m_WorkerCnt; i++) {
        int fds[2];
        if (socketpair(PF_LOCAL,SOCK_STREAM,0,fds) < 0) {
            perror("socketpair");
            return 0;
        }

        CWorker* pWorker = new CWorker();
        pWorker->m_WorkerMgr = this;
        m_WorkerList[i] = pWorker;

        AddFdToEpoll(fds[0], EPOLLIN|EPOLLET);

        ret = pthread_create(&pWorker->m_Id, NULL, WorkerMain, pWorker);
        if (ret != 0) {
            return -1;
        }
    }
}

CWorkerMgr::~CWorkerMgr()
{
    for (int i = 0; i < m_WorkerCnt; i++) {
        if (m_WorkerList[i]) {
            delete m_WorkerList[i];
        }
    }
    m_WorkerList.clear();
}

int CWorkerMgr::StartWorker()
{
    pthread_t pid;
    int ret;
    for (int i = 0; i < m_WorkerCnt; i++) {
        CWorker *pWorker = m_WorkerList[i];
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

int CWorkerMgr::PushMsg(int id, CMsg *pMsg)
{
    CWorker *pWorker = m_WorkerList[id % m_WorkerCnt];
    pWorker->PushMsg(pMsg);
    return 0;
}

}
*/
