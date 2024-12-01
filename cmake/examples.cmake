add_executable(
        example-main
        examples/logging.cpp
        examples/main.cpp
)
target_include_directories(
    example-main
    PRIVATE
    include
)
set_target_properties(
    example-main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out/examples"
)