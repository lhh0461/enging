#ifndef __WORKER_MGR__
#define __WORKER_MGR__

#include <pthread.h>
#include <list>
#include <vector>

namespace XEngine
{

//工作者
class CWorker
{
public:
    friend class CWorkerMgr;
public:
    CWorker() :m_WorkerMgr(NULL) { pthread_mutex_init(&m_Mutex, NULL); };
    ~CWorker() { pthread_mutex_destroy(&m_Mutex); };
    int ProduceTask(void *ptrTask)
    {
        pthread_mutex_lock(&m_Mutex);
        m_TaskQueue.push_back(ptrTask);
        pthread_mutex_unlock(&m_Mutex);
    }
protected:
    virtual int Init() {};
    virtual int Process(void* ptrTask) {};
private:
    pthread_t m_Id; 
    pthread_mutex_t m_Mutex;
    std::list<void *> m_TaskQueue;
    CWorkerMgr *m_WorkerMgr;
};

//工作者管理器
template <typename T>
class CWorkerMgr
{
public:
    CWorkerMgr(int iWorkerNum);
    ~CWorkerMgr();
    int StartWorker();
    int StopWorker();
    int PushTask(void *ptrTask);
    static void *WorkerMain(void *);
private:
    std::vector<CWorker *> m_Workers;
    int m_WorkerCnt;
    bool m_Quit;
    int m_Counter;
};

template <typename T>
CWorkerMgr::CWorkerMgr(int iWorkerNum)
    :m_WorkerCnt(iWorkerNum),m_Counter(0),m_Quit(0)
{
    m_Workers.reserve(iWorkerNum);
    for (int i = 0; i < m_WorkerCnt; i++) {
        T* p = new T();
        p->m_WorkerMgr = this;
        m_Workers[i] = p;
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
        void *task = NULL;
        pthread_mutex_lock(&Worker->m_Mutex);
        if (!Worker->m_TaskList.empty()) {
            task = Worker->m_TaskList.front();
        }
        pthread_mutex_unlock(&Worker->m_Mutex);
        if (task) {
            Worker->ProcessTask(task);
        }
    }
    return 0;
}

int CWorkerMgr::PushTask(void *ptrTask)
{
    CWorker *pWorker = m_Workers[++m_Count % m_WorkerCnt];
    pWorker->ProduceTask(*ptrTask);
    return 0;
}

}

#endif //__WORKER_MGR__

