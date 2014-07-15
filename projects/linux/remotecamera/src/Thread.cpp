/*
 * Thread.cpp
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "Thread.h"

const std::string stringtf( const char *format, ... )
{
    va_list ap;
    char tempBuffer[8192];
    std::string tempString;

    va_start(ap, format );
    vsnprintf( tempBuffer, sizeof(tempBuffer), format , ap );
    va_end(ap);

    tempString = tempBuffer;

    return( tempString );
}

const std::string stringtf( const std::string &format, ... )
{
    va_list ap;
    char tempBuffer[8192];
    std::string tempString;

    va_start(ap, format );
    vsnprintf( tempBuffer, sizeof(tempBuffer), format.c_str() , ap );
    va_end(ap);

    tempString = tempBuffer;

    return( tempString );
}

struct timespec getTimeout( int secs )
{
    struct timespec timeout;
    struct timeval temp_timeout;
    gettimeofday( &temp_timeout, 0 );
    timeout.tv_sec = temp_timeout.tv_sec + secs;
    timeout.tv_nsec = temp_timeout.tv_usec*1000;
    return( timeout );
}

struct timespec getTimeout( double secs )
{
    struct timespec timeout;
    struct timeval temp_timeout;
    gettimeofday( &temp_timeout, 0 );
    timeout.tv_sec = temp_timeout.tv_sec + int(secs);
    timeout.tv_nsec = temp_timeout.tv_usec += (long int)(1000000000.0*(secs-int(secs)));
    if ( timeout.tv_nsec > 1000000000 )
    {
        timeout.tv_sec += 1;
        timeout.tv_nsec -= 1000000000;
    }
    return( timeout );
}


Mutex::Mutex()
{
    if ( pthread_mutex_init( &mMutex, NULL ) < 0 )
        throw ThreadException( stringtf( "Unable to create pthread mutex: %s", strerror(errno) ) );
}

Mutex::~Mutex()
{
    if ( locked() )
        printf( "Destroying mutex when locked\n" );
    if ( pthread_mutex_destroy( &mMutex ) < 0 )
        throw ThreadException( stringtf( "Unable to destroy pthread mutex: %s", strerror(errno) ) );
}

void Mutex::lock()
{
    if ( pthread_mutex_lock( &mMutex ) < 0 )
        throw ThreadException( stringtf( "Unable to lock pthread mutex: %s", strerror(errno) ) );
}

//void Mutex::lock( int secs )
//{
//    struct timespec timeout = getTimeout( secs );
////    if ( pthread_mutex_timedlock( &mMutex, &timeout ) < 0 )
////        throw ThreadException( stringtf( "Unable to timedlock pthread mutex: %s", strerror(errno) ) );
//}
//
//void Mutex::lock( double secs )
//{
//    struct timespec timeout = getTimeout( secs );
////    if ( pthread_mutex_timedlock( &mMutex, &timeout ) < 0 )
////        throw ThreadException( stringtf( "Unable to timedlock pthread mutex: %s", strerror(errno) ) );
//}

void Mutex::unlock()
{
    if ( pthread_mutex_unlock( &mMutex ) < 0 )
        throw ThreadException( stringtf( "Unable to unlock pthread mutex: %s", strerror(errno) ) );
}

bool Mutex::locked()
{
    int state = pthread_mutex_trylock( &mMutex );
    if ( state != 0 && state != EBUSY )
        throw ThreadException( stringtf( "Unable to trylock pthread mutex: %s", strerror(errno) ) );
    if ( state != EBUSY )
        unlock();
    return( state == EBUSY );
}


Thread::Thread() :
    mThreadCondition( mThreadMutex ),
    mPid( -1 ),
    mStarted( false ),
    mRunning( false )
{
    printf( "Creating thread\n" );
}

Thread::~Thread()
{
	printf("Destroying thread %d\n", mPid );
    if ( mStarted )
        join();
}

void *Thread::mThreadFunc( void *arg )
{
	printf( "Invoking thread\n" );

    Thread *thisPtr = (Thread *)arg;
    void *status = 0;
    try
    {
        thisPtr->mThreadMutex.lock();
        thisPtr->mPid = thisPtr->id();
        thisPtr->mThreadCondition.signal();
        thisPtr->mThreadMutex.unlock();
        thisPtr->mRunning = true;
        int run=(thisPtr->run());
        status = (void *)&run;
        thisPtr->mRunning = false;
        printf( "Exiting thread, status %p", status );
    }
    catch ( const ThreadException &e )
    {
    	printf( "%s", e.getMessage().c_str() );
        thisPtr->mRunning = false;
        status = (void *)-1;
        printf( "Exiting thread after exception, status %p", status );
    }
    return( status );
}

void Thread::start()
{
	printf("Starting thread" );
    if ( isThread() )
        throw ThreadException( "Can't self start thread" );
    mThreadMutex.lock();
    if ( !mStarted )
    {
        pthread_attr_t threadAttrs;
        pthread_attr_init( &threadAttrs );
        pthread_attr_setscope( &threadAttrs, PTHREAD_SCOPE_SYSTEM );

        mStarted = true;
        if ( pthread_create( &mThread, &threadAttrs, mThreadFunc, this ) < 0 )
            throw ThreadException( stringtf( "Can't create thread: %s", strerror(errno) ) );
        pthread_attr_destroy( &threadAttrs );
    }
    else
    {
    	printf( "Attempt to start already running thread %d", mPid );
    }
    mThreadCondition.wait();
    mThreadMutex.unlock();
    printf("Started thread %d", mPid );
}

void Thread::join()
{
	printf( "Joining thread %d", mPid );
    if ( isThread() )
        throw ThreadException( "Can't self join thread" );
    mThreadMutex.lock();
    if ( mPid >= 0 )
    {
        if ( mStarted )
        {
            void *threadStatus = 0;
            if ( pthread_join( mThread, &threadStatus ) < 0 )
                throw ThreadException( stringtf( "Can't join sender thread: %s", strerror(errno) ) );
            mStarted = false;
            printf( "Thread %d exited, status %p", mPid, threadStatus );
        }
        else
        {
        	printf("Attempt to join already finished thread %d", mPid );
        }
    }
    else
    {
    	printf( "Attempt to join non-started thread %d", mPid );
    }
    mThreadMutex.unlock();
    printf( "Joined thread %d", mPid );
}

void Thread::kill( int signal )
{
    pthread_kill( mThread, signal );
}

