# Panoc Matlab interface
## Installation Windows (from source)
Requirements: 
- Visual studio with C/C++ 
- Cmake

Instructions:
1. Execute command "cmake -H. -Bbuild -DCMAKE_GENERATOR_PLATFORM=x64" in the repo directory
2. Open panoc.sln with visual studio
3. Select Release build (dropdown button that say's "Debug" by default)
4. Click on build-> build solution or press F6 (you can close Visual Studio now)
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