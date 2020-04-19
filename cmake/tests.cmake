# Download and unpack googletest at configure time
configure_file(
    cmake/CMakeLists.googletest.in
    build/download/googletest/CMakeLists.txt)
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/download/googletest )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/download/googletest )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add googletest directly to our build.
# This defines the gtest and gtest_main targets.
add_subdirectory(
    ${CMAKE_CURRENT_BINARY_DIR}/build/src/googletest
    ${CMAKE_CURRENT_BINARY_DIR}/build/googletest
    EXCLUDE_FROM_ALL)


add_executable(test-guards tests/guards.cpp)
target_include_directories(
    test-guards
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(test-guards gtest_main)    
set_target_properties(
    test-guards
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out/tests"
)    
add_test(NAME test-guards COMMAND test-guards)

add_executable(test-performance tests/performance.cpp)
target_include_directories(
    test-performance
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(test-performance gtest_main)    
set_target_properties(
    test-performance
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out/tests"
)    
add_test(NAME test-performance COMMAND test-performance)    