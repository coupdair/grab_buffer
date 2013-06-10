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
  //! number of image to record
  int image_number;
///image buffer
  //! image buffer as image list
  cimg_library::CImgList<T> image_buffer;
  ////shared index for frame count
  //!thread mutex to access to data
  pthread_mutex_t frame_mutex;
  //!frame index, i.e. current grab index
  int shared_frame_index;
///save threads
  //!pThread array
//! \todo [medium] create class for grab buffer thread
  std::vector<pthread_t> thread;
  //!thread data array
  std::vector< posixThread_data_saveBuffer<T> > thread_data;
  //!state data array
//  std::vector<bool> thread_state(thread_number);
  bool *thread_state;
  //!thread mutex to access to data
  pthread_mutex_t state_mutex;

  public:
  //! run a number of save thread(s)
  /**
   *  
  **/
  int start_save_thread(int thread_number)
  {
    if(thread_number<1) {std::cerr<<"warning: use single thread (i.e. grab thread only, no save thread)."<<std::flush;return 1;}
    //initialise thread structures
    thread.resize(thread_number);
    thread_data.resize(thread_number);
    thread_state=new bool[thread_number];
    pthread_mutex_init(&state_mutex,NULL);
    //create threads
    for(int t=0;t<thread_number;++t)
    {
      //setup data
      thread_state[t]=false;
      //setup thread data
      thread_data[t].thread_index=t+1;
      ///set thread state (i.e. both data and its mutex)
      thread_data[t].pstate_mutex=&state_mutex;
      thread_data[t].pthread_state=&(thread_state[t]);
      ///set grab image list (as shared data)
      thread_data[t].pshared_image=&image_buffer;
      ///set grab index (i.e. both data and its mutex)
      thread_data[t].pgrab_mutex=&frame_mutex;
      thread_data[t].pgrab_index=&shared_frame_index;
      //create thread
      save_buffer_posixThread<int> savebuffer;
      pthread_create(&(thread[t]),NULL,&(savebuffer.posixThread),(void*)(&thread_data[t]));
    }
    return 0;
  }//start_save_thread
  //! wait that all save thread are done
  /**
   *  \param [in] delay: try thread state each \c delay millisecond
  **/
  int wait_save_thread(int delay=12)
  {
    if(thread.size()<1) {std::cerr<<"warning: use single thread (i.e. no need to wait as no save thread).\n"<<std::flush;return 1;}
    //wait for other threads
    for(int t=0;t<thread.size();++t)
    {
      bool state;
      do
      {//wait for state==true
        //sleep a little to unload CPU
std::cerr<<"thread0: waiting for thread"<<t<<".\n"<<std::flush;
        cimg_library::cimg::wait(delay);
        //check state
        pthread_mutex_lock(&state_mutex);
        state=thread_state[t];
        pthread_mutex_unlock(&state_mutex);
      }while(!state);
      //show that thread done
      fprintf(stderr,"thread0: thread%d done.\n",t+1);
    }
    //detroy structures related to threads
    pthread_mutex_destroy(&state_mutex);
//! \todo . arrays
    delete thread_state;
    thread_data.clear();
    thread.clear();
    return 0;
  }//wait_save_thread
  //! constructor
  /**
   *  
  **/
  grab_buffer_posixThread(int image_number_in_buffer,int width=2,int height=2)
  {//constructor
    image_number=image_number_in_buffer;
    shared_frame_index=-1;
    pthread_mutex_init(&frame_mutex,NULL);
    //assign image buffer
    image_buffer.assign(image_number_in_buffer,width,height);
image_buffer.print("image buffer");
  }//constructor
  //!
  /**
   *  \return number of images recorded
  **/
  int grab_buffer(int image_number_to_record=-1)
  {//save loop
    if(image_number_to_record!=-1) image_number=image_number_to_record;
    for(int i=0;i<image_number;++i)
    {
//! \todo [grab] grab should be related to grab class
      image_buffer[i].fill(i);
std::string prefix="grab";
std::string title;title.reserve(prefix.size()+16);
title=cimg_library::cimg::number_filename(prefix.c_str(),i,3,(char*)title.c_str());
image_buffer[i].print(title.c_str());
      if(thread.size()>0)
      {//save thread running
        //set grabbed frame
        pthread_mutex_lock(&frame_mutex);
        shared_frame_index=i;
        pthread_mutex_unlock(&frame_mutex);
      }
      else
      {//single grab thread (i.e. no save thread)
        std::cerr<<"warning: single thread is running, grab thread is saving image one by one.\n"<<std::flush;
//! \todo [single] save image
        image_buffer[i].save("single_grab.cimg");
      }
    }//frame loop
    //set fake last grabbed frame for last saving in save thread(s)
    pthread_mutex_lock(&frame_mutex);
    ++shared_frame_index;
    pthread_mutex_unlock(&frame_mutex);
    return shared_frame_index;
  }//grab_buffer
  //! destructor
  /**
   *  
  **/
  ~grab_buffer_posixThread(void)
  {//destructor
    pthread_mutex_destroy(&frame_mutex);
  }//destructor
};//grab_buffer_posixThread
#endif/*GRAB_BUFFER_POSIX_THREAD*/

