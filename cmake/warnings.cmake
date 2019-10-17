set(warnings
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wunused
    -Woverloaded-virtual
    -Wnull-dereference
    -Wdouble-promotion
    -Wzero-as-null-pointer-constant
    -Wformat=2)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    list(APPEND warnings
        -Wuseless-cast
        -Wduplicated-branches
        -Wduplicated-cond
        -Wmisleading-indentation
        -Wlogical-op)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
endif()
