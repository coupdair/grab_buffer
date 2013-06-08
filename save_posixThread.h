#ifndef POSIX_THREAD_SAVE
#define POSIX_THREAD_SAVE

//POSIXThread
#include <sys/types.h>
#include "posixThread.h"

//STL
#include <iostream>
#include <vector>
//sleep
#include <unistd.h>
//using CImg library
#include "../CImg/CImg.h"

//!data shared by mainThread and posixThread.
/**
 *  
**/
template<typename T>
class posixThread_save_data: public posixThread_data
{
  public:
  //! shared image list with mainThread, i.e. image buffer
  cimg_library::CImgList<T> shared_image;
  //! grab mutex pointer: mutex lock to read grab index
  pthread_mutex_t* pgrab_mutex;
  //! grab_index pointer: value is last grabbed image index
  int* pgrab_index;
};

#endif/*POSIX_THREAD_SAVE*/

