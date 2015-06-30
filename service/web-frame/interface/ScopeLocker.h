/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file ScopeLocker.h
 * @author 122801281@qq.com
 * @date 2014/02/12 12:23:46
 * @brief 
 *  
 **/




#ifndef  __SCOPELOCK_H_
#define  __SCOPELOCK_H_

#include "pthread.h"
#include "assert.h"

namespace smart_plugin_frame{

class MutexLock
{
public:
    MutexLock()
    {
        assert(0 == pthread_mutex_init(&m_mutex, NULL));
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    void lock()
    {
        assert(0 == pthread_mutex_lock(&m_mutex));
    }
    void unlock()
    {
        assert(0 == pthread_mutex_unlock(&m_mutex));
    }
private:
    pthread_mutex_t m_mutex;
};

class ScopeLocker
{
public:
    ScopeLocker(MutexLock & mutexLock)
    {
        m_mutexLock = &mutexLock;
        m_mutexLock->lock();
    }
    ~ScopeLocker()
    {
        m_mutexLock->unlock();
    }
private:
    MutexLock * m_mutexLock;
};

}














#endif  //__SCOPELOCK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
