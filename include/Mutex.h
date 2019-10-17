#ifndef __MUTEX__
#define __MUTEX__

#include <pthread.h>

class CMutexGuard
{
public:
    CMutexGuard(pthread_mutex_t & mutex);
    ~CMutexGuard();
private:
    void Lock();
    void UnLock();
private:
     pthread_mutex_t & m_Mutex;
};

#endif //__MUTEX__
