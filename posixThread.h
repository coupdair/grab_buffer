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

#endif/*POSIX_THREAD*/

