#! /bin/bash

mkdir ../DoxIt.tmp
mv ../pivlml_pthread/*.v?.?.?.* ../DoxIt.tmp
doxygen Doxyfile
mv ../DoxIt.tmp/* ../pivlml_pthread/
rmdir ../DoxIt.tmp
