#ifndef GRAB_BUFFER_POSIX_THREAD
#define GRAB_BUFFER_POSIX_THREAD

//POSIXThread
#include <sys/types.h>
#include "save_buffer_posixThread.h"

//! grab buffer as posixThread.
/**
 *  
**/
template<typename T>
class grab_buffer_posixThread
{
  public:
  void grab_buffer(int &image_number,
    cimg_library::CImgList<T> &image_buffer,
    pthread_mutex_t &frame_mutex, int &shared_frame_index)
  {//save loop
    for(int i=0;i<image_number;++i)
    {
      image_buffer[i].fill(i);
std::string prefix="grab";
std::string title;title.reserve(prefix.size()+16);
title=cimg_library::cimg::number_filename(prefix.c_str(),i,3,(char*)title.c_str());
image_buffer[i].print(title.c_str());
      //set grabbed frame
      pthread_mutex_lock(&frame_mutex);
      shared_frame_index=i;
      pthread_mutex_unlock(&frame_mutex);
    }
    //set fake last grabbed frame for save thread(s)
    pthread_mutex_lock(&frame_mutex);
    ++shared_frame_index;
    pthread_mutex_unlock(&frame_mutex);
  }//grab_buffer
};//grab_buffer_posixThread
#endif/*GRAB_BUFFER_POSIX_THREAD*/

