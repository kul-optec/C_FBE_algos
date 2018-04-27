set(files_casadi cost_function_derivative_combined.c )
set(files_casadi "${files_casadi}" casadi_interface.c)

MESSAGE(STATUS "Compiling demo with casadi and trailer model")

add_executable(main_exe "${files_casadi}" main_trailer.c)
target_link_libraries (main_exe PANOC_lib)
if(NOT MSVC)
    target_link_libraries (main_exe m)
endif()

set_property(TARGET main_exe PROPERTY C_STANDARD 90)