cmake_minimum_required(VERSION 3.14)
project(unittests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
if(SANITIZE_BUILD OR VALGRIND_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

file(GLOB SOURCES *.cpp)
include_directories(${STLSET_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${STLSET_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${STLSET_LIBRARIES} GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME})