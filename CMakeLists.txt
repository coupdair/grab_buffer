project(grab_buffer)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/Modules/")

find_package (Threads)
find_package (X11)
#find_package (NetCDF REQUIRED)
#find_package (OpenMP REQUIRED)
#find_package (tiff REQUIRED)
#find_package (Ximea REQUIRED)


if(OPENMP_FOUND)
  message("OPENMP cmake/Modules/ found")
  message(STATUS "Found OPENMP: ${GOMP_LIBRARY}")
  message(STATUS "OPENMP FOUND")
  set(CIMG_OPENMP_CFLAGS "-Dcimg_use_openmp")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ${CIMG_OPENMP_CFLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else (OPENMP_FOUND)
   if (OPENMP_FIND_REQUIRED)
      message(FATAL_ERROR "=========> Could NOT find OPENMP library")
   endif (OPENMP_FIND_REQUIRED)
endif (OPENMP_FOUND)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -ansi -pedantic -O3 -fno-tree-pre")

#IF(XIMEA_FOUND)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm3api")
#else (XIMEA_FOUND)
#  message(FATAL_ERROR "=========> Could NOT find Ximea library")
#endif (XIMEA_FOUND)  
# 
# IF(TIFF_FOUND)
#   message("Tiff cmake/Modules/ found")
#   #   cimg use vt100 (color), verbosity (message out-of-bound matrix)
#   set(CIMG_TIFF_FLAGS "-Dcimg_use_tiff")
#   set(CIMG_TIFF_LDFLAGS "-ltiff")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_TIFF_FLAGS}")
#   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CIMG_TIFF_LDFLAGS}")
# else (TIFF_FOUND)
#   message(FATAL_ERROR "=========> Could NOT find Tiff library")
# endif (TIFF_FOUND)  



# if (NOT NETCDF_FOUND)
#    if (NETCDF_FIND_REQUIRED)
#       message(STATUS "Looked for NETCDF libraries named ${NETCDF_NAMES}.")
#       message(STATUS "Include file detected: [${NETCDF_INCLUDE_DIR}].")
#       message(STATUS "Lib file detected: [${NETCDF_LIBRARY}].")
#       message(FATAL_ERROR "=========> Could NOT find NETCDF library")
#    endif (NETCDF_FIND_REQUIRED)
# endif (NOT NETCDF_FOUND)


# add the executable
add_executable(grab_buffer main.cpp)

# target_link_libraries (grabber ${CMAKE_THREAD_LIBS_INIT} ${X11_X11_LIB} ${GOMP_LIBRARY} ${XIMEA_LIBRARIES} ${NETCDF_LIBRARIES} ${TIFF_LIBRARIES})
target_link_libraries (grab_buffer ${CMAKE_THREAD_LIBS_INIT} ${X11_X11_LIB} ${GOMP_LIBRARY} ${XIMEA_LIBRARIES})

