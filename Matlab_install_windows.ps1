Write-Host "Creating Visual Studio build system in ./build folder"
Write-Host "Make sure you have Visual Studio with C/C++ tools  and Cmake installed"
cmake . -Bbuild -DCMAKE_GENERATOR_PLATFORM=x64

Write-Host "Compiling 64 bit share library used by the mex file"
cmake --build ./build --config Release
cp .\build\PANOC\Release\PANOC_lib.lib .\Matlab\PANOC_lib.lib

Write-Host "Compiling the mex file"
mex  .\Matlab\panoc.c .\Matlab\panoc_arguments_check.c .\Matlab\panoc_arguments_parse.c .\Matlab\PANOC_lib.lib -outdir .\Matlab