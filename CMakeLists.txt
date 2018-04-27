### make sure the user is doing an our of source build:
 if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
     message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
 endif()
###

cmake_minimum_required (VERSION 2.6)
project (panoc C)

# set the compiler with its flags
# SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_BUILD_TYPE Debug)
if(NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fprofile-arcs -ftest-coverage")
    set(LDFLAGS "-O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
endif()

# subdirectories with libraries
include_directories ("${PROJECT_SOURCE_DIR}/PANOC")
add_subdirectory (PANOC) # read out the cmakelist file

# subdirectories with demos
include_directories ("${PROJECT_SOURCE_DIR}/demo_nmpc_casadi")
add_subdirectory (demo_nmpc_casadi) # read out the cmakelist file

# Test the functionality
enable_testing()
add_subdirectory(test)

# Tests on the matrix/vector operations 
add_test(NAME lbfgs_poly_test COMMAND  test/lbfgs_poly_test)
add_test(NAME prox_grad_descent_poly_test COMMAND  test/prox_grad_descent_poly_test)
add_test(NAME panoc_test COMMAND  test/panoc_test)