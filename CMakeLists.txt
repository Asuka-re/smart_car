cmake_minimum_required(VERSION 3.16)
project(SmartCar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 业务库
add_library(executor STATIC
    src/executor.cpp
    src/PoseHandler.cpp
    include/executor.h
    src/PoseHandler.hpp
    src/Command.hpp
)
target_include_directories(executor PUBLIC include src)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# 测试
add_executable(ExecutorTest tests/ExecutorTest.cpp)
target_link_libraries(ExecutorTest PRIVATE executor gtest_main)
enable_testing()
add_test(NAME ExecutorTest COMMAND ExecutorTest)