#!/bin/bash
echo "Creating Makefiles  in ./build folder"
cmake . -Bbuild

echo "Compiling shared library used by the mex file"
cmake --build ./build
cp ./build/PANOC/libPANOC_lib.dylib ./Matlab/libPANOC_lib.dylib

echo "Compiling the mex file"
mex  ./Matlab/panoc.c ./Matlab/panoc_arguments_check.c ./Matlab/panoc_arguments_parse.c ./Matlab/libPANOC_lib.dylib -outdir ./Matlab