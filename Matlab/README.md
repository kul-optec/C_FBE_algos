# Panoc Matlab interface
## Automatic installation
3 scripts are avaialable in the root folder, that automate this process:
1. Matlab_install_windows.ps1 (uses powershell, visual studio and cmake)
2. Matlab_install_linux.sh (uses bash, cmake and make)
3. Matlab_install_mac.sh (uses bash, cmake and make)

## Installation Windows (from source)
Requirements: 
- Visual studio with C/C++ 
- Cmake

Instructions:
1. Execute command "cmake -H. -Bbuild -DCMAKE_GENERATOR_PLATFORM=x64" in the repo directory
2. Execute command "cmake --build ./build --config Release" in the repo directory
5. Copy build/PANOC/PANOC_lib.lib into the Matlab folder
6. Open Matlab in the Matlab folder
7. Execute "mex panoc.c panoc_arguments_check.c panoc_arguments_parse.c PANOC_lib.lib" in Matlab
8. Add the Matlab folder to your path

## Installation Linux (from source)
Requirements: 
- Cmake
- Make
- C-compiler

Instructions:
1. Execute command "cmake -H. -Bbuild"
2. Go into the build directory and execute "make all" command
3. Copy build/PANOC/PANOC_lib.so into the Matlab folder
4. Open Matlab in the Matlab folder
5. Execute "mex panoc.c panoc_arguments_check.c panoc_arguments_parse.c PANOC_lib.so" in Matlab
6. Add the Matlab folder to your path

## Installation Mac (from source)
Requirements: 
- Cmake
- Make
- C-compiler

Instructions:
1. Execute command "cmake -H. -Bbuild"
2. Go into the build directory and execute "make all" command
3. Copy build/PANOC/PANOC_lib.dylib into the Matlab folder
4. Open Matlab in the Matlab folder
5. Execute "mex panoc.c panoc_arguments_check.c panoc_arguments_parse.c PANOC_lib.dylib" in Matlab
6. Add the Matlab folder to your path