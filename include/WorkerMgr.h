#ifndef __WORKER_MGR__
#define __WORKER_MGR__

#include <pthread.h>
#include <list>
#include <vector>
#include "WorkerFactory.h"

namespace XEngine
{

class CWorkerMgr;

//工作者(继承用)
class CWorker
{
public:
    friend class CWorkerMgr;
public:
    CWorker();
    ~CWorker();
protected:
    virtual int Init() {};
    virtual int Process(void *pTask) {};
private:
    int ProduceTask(void *pTask);
private:
    pthread_t m_Id; 
    pthread_mutex_t m_Mutex;
    std::list<void *> m_TaskQueue;
    CWorkerMgr *m_WorkerMgr;
    void *m_UserData;
};

//工作者管理器
class CWorkerMgr
{
public:
    CWorkerMgr(int iWorkerNum, eWorkerType eType);
    ~CWorkerMgr();
    int StartWorker();
    int StopWorker();
    int PushTask(void *pTask);
    static void *WorkerMain(void *);
private:
    std::vector<CWorker *> m_Workers;
    int m_WorkerCnt;
    eWorkerType m_WorkerType;
    bool m_Quit;
    unsigned m_Counter;

};

}

#endif //__WORKER_MGR__

