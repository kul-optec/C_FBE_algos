# lib with differnt f and g's in to use in tests
add_library(problem_set STATIC example_problems.c)
set_property(TARGET problem_set PROPERTY C_STANDARD 90)

# lib used when testing the lbfgs with polynomial, it replaces the prox grad descent with a mock
add_library(PANOC_test_lbfgs STATIC "${PROJECT_SOURCE_DIR}/PANOC/lbfgs.c" "${PROJECT_SOURCE_DIR}/PANOC/matrix_operations.c" mocks/lbfgs_test_proximal_gradient_descent.c mocks/lbfgs_test_buffer.c )
set_property(TARGET PANOC_test_lbfgs PROPERTY C_STANDARD 90)

add_executable(lbfgs_poly_test lbfgs_poly_test.c)
target_link_libraries (lbfgs_poly_test PANOC_test_lbfgs)
if(NOT MSVC) # Microsoft Visual studio doesn't like this
    target_link_libraries(lbfgs_poly_test m) # link with the math lib, some toolchains need this
endif()
target_link_libraries(lbfgs_poly_test problem_set)

add_executable(prox_grad_descent_poly_test prox_grad_descent_poly_test.c)
target_link_libraries (prox_grad_descent_poly_test PANOC_fake)
if(NOT MSVC) # Microsoft Visual studio doesn't like this
    target_link_libraries(prox_grad_descent_poly_test m) # link with the math lib, some toolchains need this
endif()
target_link_libraries(prox_grad_descent_poly_test problem_set)

add_executable(panoc_test panoc_test.c)
target_link_libraries (panoc_test PANOC_lib)
if(NOT MSVC) # Microsoft Visual studio doesn't like this
    target_link_libraries(panoc_test m) # link with the math lib, some toolchains need this
endif()
target_link_libraries(panoc_test problem_set)