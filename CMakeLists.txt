cmake_minimum_required(VERSION 3.1)

project(stlset)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS}")

option(SANITIZE_BUILD OFF)
option(VALGRIND_BUILD OFF)
message("SANITIZE_BUILD = ${SANITIZE_BUILD}")

if (SANITIZE_BUILD)
    message("Use sanitizer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error -coverage -lgcov -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all -O0")
endif()
if (VALGRIND_BUILD)
    message("use valgrind")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif()
message("CMAKE_EXE_LIKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_CXX_COMPILE_FLAGS = ${CMAKE_CXX_COMPILE_FLAGS}")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(STLSET_INCLUDE_DIRS ${INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})

enable_testing()
add_subdirectory(tests)