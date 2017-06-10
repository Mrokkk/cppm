cmake_minimum_required(VERSION 3.0)

set(CPP_STD "c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CPP_STD} -Wall -Wextra -Werror")

option(COVERAGE "Generate coverage reports" OFF)
message(STATUS "[COVERAGE] Generate coverage reports: ${COVERAGE}")

option(SANITIZERS "Use asan and ubsan" OFF)
message(STATUS "[SANITIZERS] Use asan and ubsan: ${SANITIZERS}")

add_subdirectory(src)
add_subdirectory(test)

