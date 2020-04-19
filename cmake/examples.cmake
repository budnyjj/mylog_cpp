add_executable(example-main examples/main.cpp)
target_include_directories(
    example-main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
    example-main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out/examples"
)