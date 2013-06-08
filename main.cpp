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
#include "save_posixThread.h"

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
  const int image_number=cimg_option("-n",12,"number of image to record.");
        int image_number_in_buffer=cimg_option("-b",1,"number of image in buffer.");
//temporary values
  thread_number=1;
  image_number_in_buffer=image_number;
  //!pThread array
//! \todo [medium] create class for grab buffer thread
  std::vector<pthread_t> thread(thread_number);
  //!thread data array
  std::vector< posixThread_save_data<int> > thread_data(thread_number);
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
  cimg_library::CImgList<int> image_buffer(image_number_in_buffer);
  //create threads
  for(int t=0;t<thread_number;++t)
  {
    //setup data
    thread_state[t]=false;
    //setup thread data
    thread_data[t].thread_index=t+1;
    ///thread state
    thread_data[t].pmutex=&mutex;
    thread_data[t].pthread_state=&(thread_state[t]);
    ///grab index
    thread_data[t].pgrab_mutex=&frame_mutex;
    thread_data[t].pgrab_index=&shared_frame_index;
    //create thread
    pthread_create(&(thread[t]),NULL,&(posixThread_save/*<int>*/::posixThread),(void*)(&thread_data[t]));
  }
  //grab loop
//! \todo [high] . grab loop
  for(int i=0;i<image_number;++i)
  {
    image_buffer[i].assign(234,123,1,1,i);
    //set grabbed frame
    pthread_mutex_lock(&frame_mutex);
    shared_frame_index=i;
    pthread_mutex_unlock(&frame_mutex);
  }
  //for frame
//! \todo [high] . wait state true for all (or use pThread wait all thread)
  //wait for other threads
  for(int t=0;t<thread_number;++t)
  {
    bool state;
    do
    {//wait for state==true
      //check state
      pthread_mutex_lock(&mutex);
      state=thread_state[t];
      pthread_mutex_unlock(&mutex);
      //sleep a little to unload CPU
      cimg_library::cimg::wait(12);
    }while(!state);
    //show that thread done
    fprintf(stderr,"thread0: thread%d done.\n",t);
  }
  pthread_mutex_destroy(&mutex);
  pthread_exit(0);
}//main

