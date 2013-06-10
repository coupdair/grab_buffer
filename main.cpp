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
  //grab in buffer
  grab_buffer_posixThread<int> grabuffer(image_number_in_buffer,321,123);
  //save buffer using other thread(s)
  grabuffer.start_save_thread(thread_number);//unblocking
  //grab init
//load CPU, so other thread are waiting a little
for(int j=0;j<image_number;++j) for(int i=0;i<image_number;++i) grabuffer.image_buffer[i].fill(0.123*(i*j+1)).cos().fill(1.23*(i*j+1)).sin().fill(12.3*(i*j+1)).tan();
  //grab loop
  grabuffer.grab_buffer(image_number);//blocking
  //wait for other threads
  grabuffer.wait_save_thread(12);
  pthread_exit(0);
}//main

