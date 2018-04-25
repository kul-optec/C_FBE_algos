file(GLOB panoc_SRC
    "*.c"
)
add_library(PANOC_lib STATIC ${panoc_SRC})
set_property(TARGET PANOC_lib PROPERTY C_STANDARD 90)

# this is nessesary for the interfaces to other languages
set_property(TARGET PANOC_lib PROPERTY POSITION_INDEPENDENT_CODE ON) 

add_library(PANOC_fake STATIC lbfgs.c lipschitz.c matrix_operations.c panoc.c proximal_gradient_descent.c buffer.c "${PROJECT_SOURCE_DIR}/test/mocks/function_evaluator_test.c")
set_property(TARGET PANOC_fake PROPERTY C_STANDARD 90)