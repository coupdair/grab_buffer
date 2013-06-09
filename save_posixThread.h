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
  cimg_library::CImgList<T> *pshared_image;
  //! grab mutex pointer: mutex lock to read grab index
  pthread_mutex_t* pgrab_mutex;
  //! grab_index pointer: value is last grabbed image index
  int* pgrab_index;
//! \todo \c save_index not used, yet ! (should be used for buffer < record image sizes)
  //! last saved image index
  int save_index;
  //! constructor
  /**
   *
  **/
  posixThread_save_data()
  {
    save_index=-1;
  }//constructor
};//posixThread_save_data

//! posixThread.
/**
 *  
**/
//! \todo [medium] v try to put template back
//! \todo [medium] try to remove pointer with get function in class
template<typename T>
class posixThread_save
{
  public:
  static void exec(posixThread_save_data<T> &shared_data)
  {//save loop
    fprintf(stderr,"thread%d: save_index=%d\n",shared_data.thread_index,shared_data.save_index);
    for(int i=0;i<shared_data.pshared_image->size();++i)
    {
      int grab_index;
      do
      {//wait i<grab index
std::cerr<<"thread"<<shared_data.thread_index<<": waiting for image#"<<i<<".\n"<<std::flush;
        //sleep a little to unload CPU
        cimg_library::cimg::wait(12);
        //get last grabbed frame
        pthread_mutex_lock(shared_data.pgrab_mutex);
        grab_index=*(shared_data.pgrab_index);
        pthread_mutex_unlock(shared_data.pgrab_mutex);
      }while(!(i<grab_index));
      //save grabbed image
std::string image_name="grab.cimg";
      std::string filename;filename.reserve(image_name.size()+16);
      filename=cimg_library::cimg::number_filename(image_name.c_str(),i,3,(char*)filename.c_str());
(*(shared_data.pshared_image))[i].print(filename.c_str());
      (*(shared_data.pshared_image))[i].save(filename.c_str());
      std::cerr<<"thread"<<shared_data.thread_index<<": "<<filename<<" saved.\n";
    }//save loop
  }//exec
//! \todo [next] setup &shared_data as for exec
  static void stop(posixThread_save_data<T>* pdata)
  {
    pthread_mutex_lock(pdata->pmutex);
fprintf(stderr,"thread%d_data=(index=%d,state=%s)\n",pdata->thread_index,pdata->thread_index,(*(pdata->pthread_state))?"true":"false");
    *(pdata->pthread_state)=true;//computation done
fprintf(stderr,"thread%d_data=(index=%d,state=%s)\n",pdata->thread_index,pdata->thread_index,(*(pdata->pthread_state))?"true":"false");
    pthread_mutex_unlock(pdata->pmutex);
    //stop thread
    pthread_exit(0);
  }
  //!thread computes in this function.
  /**
   *  function return an integer, it should be 0 if it terminates normaly.
   *  \param ptr = this pointer should be a posixThread_data*.
  **/
static  void* posixThread(void* ptr)
  {
//init pdata (could be class member ?)
fprintf(stderr,"posixThread_save\n");
    posixThread_save_data<T>* pdata=(posixThread_save_data<T>*)ptr;
//fprintf(stderr,"thread%d  ptr =%p\n",0,ptr);
//fprintf(stderr,"&thread%d_data=%p\n",pdata->thread_index,(void*)pdata);
(*(pdata->pshared_image)).print("image buffer in thread");
//computations i.e. exec function
    //save loop
    exec(*pdata);
//! \todo [medium] should be in parent class
//Thread ending i.e. set state and stop
    stop(pdata);
  }//posixThread
};//posixThread_save
#endif/*POSIX_THREAD_SAVE*/

