/*-----------------------------------------------------------------------

  File        : grab_buffer.cpp

  Description : grab in buffer for either image processing or save

  Author      : Sebastien COUDERT

  --------------------------------------------------------------------*/

/*Additional documentation for the generation of the reference page (using doxygen)*/
/** \mainpage
 *
 *  \par Doxygen brief:
 *
 *  Each function in the source code should be commented using \b doxygen \b syntax in the same file.
 *  The documentation need to be written before the function.
 *  The basic syntax is presented in this part.
 *  \verbinclude "doxygen.example1.txt"
 *
 *  Two kind of comments are needed for \b declaration and \b explanation \b parts of the function:
 *  Standart documentation should the following (\b sample of code documentation):
 *  \verbinclude "doxygen.example2.txt"
 *
 *  In both declaration and explanation part, \b writting and \b highlithing syntax can be the following:\n\n
 *  \li \c \\code to get\n
 *
 *  \li \c \\n    a new line
 *  \li \c \\li   a list (dot list)
 *
 *  \li \c \\b    bold style
 *  \li \c \\c    code style
 *  \li \c \\e    enhanced style (italic)
 *
 *  For making \b shortcut please use:\n
 *  \li \c \\see to make a shortcut to a related function or variable
 *  \li \c \\link to make a shortcut to a file or a function
 *  \note this keyword needs to be closed using \c \\end*
 *
 *  \li \c \\todo to add a thing to do in the list of <a href="todo.html">ToDo</a> for the whole program
 *
 *  In explanation part, \b paragraph style can be the following:\n
 *  \li \c \\code for an example of the function use
 *  \li \c \\note to add a few notes
 *  \li \c \\attention for SOMETHING NOT FULLY DEFINED YET
 *  \li \c \\warning to give a few warning on the function
 *  \note these keywords need to be closed using \c \\end*
 *
 *  \verbinclude "doxygen.example3.txt"
 *
 *  Many other keywords are defined, so please read the documentation of <a href="http://www.doxygen.org/doc/">doxygen</a>.
 *
**/

//CMake config
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//POSIXThread
#include <sys/types.h>
#include "grab_buffer_posixThread.h"

//!the program starts in this main function.
/**
 *  function return an integer, it should be 0 if it terminates normaly.
 *  \param argc = number of command line argument.
 *  \param argv = command line argument vector (should be \c argv).
**/
int main(int argc,char **argv)
{
  //option help
  cimg_usage("grab_buffer");
  //initialise threads
  int thread_number=cimg_option("-t",1,"number of thread to run range=[0..[.");
  int image_number=cimg_option("-n",12,"number of image to record.");
  int image_number_in_buffer=cimg_option("-b",1,"number of image in buffer.");
//temporary values
std::cerr<<"warning development version:\n";
  thread_number=1;
std::cerr<<"- main and "<<thread_number<<" thread only.\n"<<std::flush;
  image_number_in_buffer=image_number;
std::cerr<<"- buffer and recorded image numbers are the same.\n"<<std::flush;
std::cerr<<std::flush;
  //!pThread array
//! \todo [medium] create class for grab buffer thread
  std::vector<pthread_t> thread(thread_number);
  //!thread data array
  std::vector< posixThread_data_saveBuffer<int> > thread_data(thread_number);
  //!state data array
//  std::vector<bool> thread_state(thread_number);
  bool *thread_state=new bool[thread_number];
  //!thread mutex to access to data
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex,NULL);

  //!frame index, i.e. current grab index
  int shared_frame_index=-1;
  //!thread mutex to access to data
  pthread_mutex_t frame_mutex;
  pthread_mutex_init(&frame_mutex,NULL);

  //!image buffer
  cimg_library::CImgList<int> image_buffer(image_number_in_buffer,234,123);
image_buffer.print("image buffer");
/*
  const cimg_library::CImgList<int> image_buffer_shared=image_buffer.get_shared();
image_buffer_shared.print("image buffer shared");
image_buffer.print("image buffer");
*/
  //create threads
  for(int t=0;t<thread_number;++t)
  {
    //setup data
    thread_state[t]=false;
    //setup thread data
    thread_data[t].thread_index=t+1;
    ///set thread state (i.e. both data and its mutex)
    thread_data[t].pstate_mutex=&mutex;
    thread_data[t].pthread_state=&(thread_state[t]);
    ///set grab image list (as shared data)
    thread_data[t].pshared_image=&image_buffer;
/*
    thread_data[t].shared_image.assign(image_number_in_buffer);
thread_data[t].shared_image.print("shared_image 1");
    cimglist_for(image_buffer,i) (thread_data[t].shared_image[i])=image_buffer[i].get_shared_channel(0);
thread_data[t].shared_image.print("shared_image 2");
*/
    ///set grab index (i.e. both data and its mutex)
    thread_data[t].pgrab_mutex=&frame_mutex;
    thread_data[t].pgrab_index=&shared_frame_index;
    //create thread
    save_buffer_posixThread<int> savebuffer;
    pthread_create(&(thread[t]),NULL,&(savebuffer.posixThread),(void*)(&thread_data[t]));
  }
  //grab init
//load CPU, so other thread are waiting a little
for(int j=0;j<image_number;++j) for(int i=0;i<image_number;++i) image_buffer[i].fill(0.123*(i*j+1)).cos().fill(1.23*(i*j+1)).sin().fill(12.3*(i*j+1)).tan();
  //grab loop
  grab_buffer_posixThread<int> grabuffer;
  grabuffer.grab_buffer(image_number,image_buffer,frame_mutex,shared_frame_index);
//! \todo [high] . wait state true for all (or use pThread wait all thread)
  //wait for other threads
  for(int t=0;t<thread_number;++t)
  {
    bool state;
    do
    {//wait for state==true
      //sleep a little to unload CPU
std::cerr<<"thread0: waiting for thread"<<t<<".\n"<<std::flush;
      cimg_library::cimg::wait(12);
      //check state
      pthread_mutex_lock(&mutex);
      state=thread_state[t];
      pthread_mutex_unlock(&mutex);
    }while(!state);
    //show that thread done
    fprintf(stderr,"thread0: thread%d done.\n",t+1);
  }
  pthread_mutex_destroy(&mutex);
  pthread_exit(0);
}//main

