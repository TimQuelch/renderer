cmake_minimum_required(VERSION 3.10)
project(renderer)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/warnings.cmake)

find_package(Catch2 REQUIRED)
include(Catch)

add_executable(tests test-main.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2)
target_compile_options(tests PRIVATE ${warnings})
catch_discover_tests(tests)
