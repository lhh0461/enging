#include "Mutex.h"

CMutexGuard::CMutexGuard(pthread_mutex_t & mutex)
    :m_Mutex(mutex)
{
    Lock();
}

CMutexGuard::~CMutexGuard()
{
    UnLock();
}

void CMutexGuard::Lock()
{
    pthread_mutex_lock(&m_Mutex);
}

void CMutexGuard::UnLock()
{
    pthread_mutex_unlock(&m_Mutex);
}
