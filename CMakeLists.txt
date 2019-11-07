cmake_minimum_required(VERSION 3.10)
project(renderer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RENDERER_ENABLE_TESTING "Enable unit tests" ON)
option(RENDERER_RUN_CLANG_TIDY "Run clang tidy on C++ code" OFF)

# Include conan files if they exist
include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)

# Find dependencies
find_package(fmt REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenMP REQUIRED)

# Set compile options
include(${CMAKE_SOURCE_DIR}/cmake/warnings.cmake)
set(optimisation_flags_release -O3 -march=native -mtune=native)
set(optimisation_flags_debug -O0)
set(debug_flags -g)
add_compile_options(
    ${warnings}
    "$<$<CONFIG:Debug>:${optimisation_flags_debug}>"
    "$<$<CONFIG:Debug>:${debug_flags}>"
    "$<$<CONFIG:Release>:${optimisation_flags_release}>")

# Enable link time optimisation (interprocedural optimisation) if available
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORTED)
if (LTO_SUPPORTED)
    message(STATUS "LTO enabled for release modes")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
else()
    message(STATUS "LTO not supported")
endif()

# Link TBB if using GCC for parallel STL
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    link_libraries(tbb)
endif()

# Set up clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found.")
    set(
        CLANG_TIDY
        "${CLANG_TIDY_EXE}")
endif()

# Add main lib target
add_library(renderer
    src/renderer.cpp
    src/camera.cpp
    src/png-write.cpp
    src/scene.cpp
    src/primative.cpp
    src/frame.cpp
    src/colour.cpp)
target_link_libraries(renderer
    PUBLIC
    fmt::fmt
    Eigen3::Eigen
    nlohmann_json::nlohmann_json
    PNG::PNG
    OpenMP::OpenMP_CXX)
target_include_directories(renderer PUBLIC src/)
target_compile_features(renderer PUBLIC cxx_std_17)

# Add main app target
add_executable(renderer-app src/main.cpp)
target_link_libraries(renderer-app PUBLIC renderer)

# Copy over data
configure_file(data/render-config.json ./ COPYONLY)

# Add tests
if(${RENDERER_ENABLE_TESTING})
    enable_testing()
    add_subdirectory(test)
endif()

# Install
include(GNUInstallDirs)

install(
    TARGETS renderer-app
    EXPORT renderer-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
