/*
 * Thread.h
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

//#include <sys/syscall.h>

#include "Exception.h"

class ThreadException : public Exception
{
public:
    ThreadException( const std::string &message )
    : Exception(NULL )
    //: Exception( stringtf( "(%d) "+message, (long int)syscall(SYS_gettid) ) )
    {

    }
};


class Mutex
{
friend class Condition;

private:
    pthread_mutex_t mMutex;

public:
    Mutex();
    ~Mutex();

private:
    pthread_mutex_t *getMutex()
    {
        return( &mMutex );
    }

public:
    void lock();
    void lock( int secs );
    void lock( double secs );
    void unlock();
    bool locked();
};



class Condition
{
private:
    Mutex &mMutex;
    pthread_cond_t mCondition;

public:
    Condition( Mutex &mutex );
    ~Condition();

    void wait();
    bool wait( int secs );
    bool wait( double secs );
    void signal();
    void broadcast();
};


class Semaphore : public Condition
{
private:
    Mutex mMutex;

public:
    Semaphore() : Condition( mMutex )
    {
    }

    void wait()
    {
        mMutex.lock();
        Condition::wait();
        mMutex.unlock();
    }
    bool wait( int secs )
    {
        mMutex.lock();
        bool result = Condition::wait( secs );
        mMutex.unlock();
        return( result );
    }
    bool wait( double secs )
    {
        mMutex.lock();
        bool result = Condition::wait( secs );
        mMutex.unlock();
        return( result );
    }
    void signal()
    {
        mMutex.lock();
        Condition::signal();
        mMutex.unlock();
    }
    void broadcast()
    {
        mMutex.lock();
        Condition::broadcast();
        mMutex.unlock();
    }
};


class Thread
{
public:
    typedef void *(*ThreadFunc)( void * );

protected:
    pthread_t mThread;

    Mutex mThreadMutex;
    Condition mThreadCondition;
    pid_t mPid;
    bool  mStarted;
    bool  mRunning;

protected:
    Thread();
    virtual ~Thread();

    pid_t id() const
    {
        //return( (pid_t)syscall(SYS_gettid) );
    	return 0;
    }
    void exit( int status = 0 )
    {
        //INFO( "Exiting" );
        pthread_exit( (void *)&status );
    }
    static void *mThreadFunc( void *arg );

public:
    virtual int run() = 0;

    void start();
    void join();
    void kill( int signal );
    bool isThread()
    {
        return( mPid > -1 && pthread_equal( pthread_self(), mThread ) );
    }
    bool isStarted() const { return( mStarted ); }
    bool isRunning() const { return( mRunning ); }
};



#endif /* THREAD_H_ */
