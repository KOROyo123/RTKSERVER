#ifndef KORO_H
#define KORO_H


#include <math.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <pthread.h>
#include <sys/select.h>
#endif


#define MAXSTR      1024                /* max length of a stream */
#define MAXPATH     1024                /* max length of a filepath */



#ifdef WIN32
#define thread_t    HANDLE
#define lock_t      CRITICAL_SECTION
#define initlock(f) InitializeCriticalSection(f)
#define lock(f)     EnterCriticalSection(f)
#define unlock(f)   LeaveCriticalSection(f)
#define FILEPATHSEP '\\'
#else
#define thread_t    pthread_t
#define lock_t      pthread_mutex_t
#define initlock(f) pthread_mutex_init(f,NULL)
#define lock(f)     pthread_mutex_lock(f)
#define unlock(f)   pthread_mutex_unlock(f)
#define FILEPATHSEP '/'
#endif






namespace koro
{
    int char2arg(char *str, int *argc, char **argv, int max_argc);

    void llh2xyz(double &x, double &y, double &z);
    void xyz2llh(double &x, double &y, double &z);

    void enu2pose(double *p0, double *p1, double *p2,double *pose);
};

#endif // KORO_H


