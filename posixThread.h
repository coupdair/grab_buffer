#ifndef POSIX_THREAD
#define POSIX_THREAD

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//!data shared by mainThread and posixThread.
/**
 *  this data structure enable all shared data by mainThread and posixThread.
**/
struct posixThread_data
{
  //! thread index: value < number of threads (information only)
  int thread_index;
  //! mutex pointer: mutex lock to read and write thread_state (not needed if task is a single block, used for debugging purpose)
  pthread_mutex_t* pmutex;
  //! thread state pointer: value is true computation successfull (used for debugging purpose)
  bool* pthread_state;
};

//!thread computes in this function.
/**
 *  function return an integer, it should be 0 if it terminates normaly.
 *  \param ptr = this pointer should be a posixThread_data*.
**/
/*
void* posixThread(void* ptr)
{
fprintf(stderr,"posixThread\n");
  posixThread_data* pdata=(posixThread_data*)ptr;
//fprintf(stderr,"thread%d  ptr =%p\n",0,ptr);
//fprintf(stderr,"&thread%d_data=%p\n",pdata->thread_index,(void*)pdata);
  //computations
  //save loop
//! \todo [high] _ save loop
  fprintf(stderr,"thread%d\n",pdata->thread_index);
  //Thread ending
  pthread_mutex_lock(pdata->pmutex);
fprintf(stderr,"thread%d_data=(index=%d,state=%s)\n",pdata->thread_index,pdata->thread_index,(*(pdata->pthread_state))?"true":"false");
  *(pdata->pthread_state)=true;//computation done
fprintf(stderr,"thread%d_data=(index=%d,state=%s)\n",pdata->thread_index,pdata->thread_index,(*(pdata->pthread_state))?"true":"false");
  pthread_mutex_unlock(pdata->pmutex);
  //stop thread
  pthread_exit(0);
}//posixThread
*/
#endif/*POSIX_THREAD*/

