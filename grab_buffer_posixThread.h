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
  int image_number;
  cimg_library::CImgList<T> image_buffer;
  //! shared index for frame count
  pthread_mutex_t frame_mutex;
  int shared_frame_index;
  public:
  //!
  /**
   *  
  **/
  grab_buffer_posixThread(int image_number_in_buffer,int width=0,int height=0)
  {//constructor
    //!frame index, i.e. current grab index
    shared_frame_index=-1;
    //!thread mutex to access to data
    pthread_mutex_init(&frame_mutex,NULL);
    //!image buffer
    image_buffer.assign(image_number_in_buffer,width,height);
image_buffer.print("image buffer");
  }//constructor
  //!
  /**
   *  
  **/
  void grab_buffer(int &image_number)
  {//save loop
    for(int i=0;i<image_number;++i)
    {
//! \todo [grab] grab should be related to grab class
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
    //set fake last grabbed frame for last saving in save thread(s)
    pthread_mutex_lock(&frame_mutex);
    ++shared_frame_index;
    pthread_mutex_unlock(&frame_mutex);
  }//grab_buffer
  //!
  /**
   *  
  **/
  ~grab_buffer_posixThread(void)
  {//destructor
    pthread_mutex_destroy(&frame_mutex);
  }//destructor
};//grab_buffer_posixThread
#endif/*GRAB_BUFFER_POSIX_THREAD*/

