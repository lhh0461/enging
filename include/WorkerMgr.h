/*
#ifndef __WORKER_MGR__
#define __WORKER_MGR__

#include <pthread.h>
#include <list>
#include <vector>
#include "WorkerFactory.h"

namespace XEngine
{

class CPackage;
class CWorkerMgr;

struct WorkerType
{

}

//工作者(继承用)
class CWorker
{
public:
    CWorker();
    ~CWorker();
protected:
    virtual int Init() {};
    virtual int Process(void *pTask) {};
private:
    int PushMsg(CMsg *pMsg);
    int PushMsg(CMsg *pMsg);
private:
    pthread_t m_Id; 
    int sockfd; 
    CWorkerMgr *m_WorkerMgr;
};

//工作者管理器
class CWorkerMgr
{
public:
    CWorkerMgr(int iWorkerNum, eWorkerType eType);
    ~CWorkerMgr();
    int InitWorker();
    int StartWorker();
    int StopWorker();
    int PushMsg(int id, CMsg *pMsg);
    static void *WorkerMain(void *);
private:
    WorkerType m_WorkerType;
    std::vector<CWorker *> m_WorkerList;
    int m_WorkerCnt;
    bool m_Quit;
};

}

#endif //__WORKER_MGR__

*/
