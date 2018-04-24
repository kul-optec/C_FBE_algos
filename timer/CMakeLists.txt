# add a simple timer lib, usefull to time the convergence
if(WIN32)
    SET(SRC_TIMER_LIB "timer_windows.c")
elseif(APPLE )
    SET(SRC_TIMER_LIB "timer_mac.c" ) 
else()
    SET(SRC_TIMER_LIB "timer_linux.c" )
endif()

add_library(TIMER_LIB SHARED "${SRC_TIMER_LIB}")