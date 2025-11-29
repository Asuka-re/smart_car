cmake_minimum_required(VERSION 3.16)

# ✅ 指定编译器（根据您的实际路径）
set(CMAKE_C_COMPILER "C:/mingw64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/mingw64/mingw64/bin/g++.exe")

project(SmartCar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)
# Optional debug prints for grouping; toggle ON to trace
# 移除 SMARTCAR_DEBUG 以避免 MinGW /D 风格参数问题；需要调试时再手动启用
# add_compile_definitions(SMARTCAR_DEBUG)
if (CMAKE_CXX_FLAGS MATCHES "/DSMARTCAR_DEBUG")
    string(REPLACE "/DSMARTCAR_DEBUG" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    message(STATUS "Stripped stray /DSMARTCAR_DEBUG from CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

set(EXECUTOR_SOURCES
    src/executor.cpp
    src/PoseHandler.cpp
    src/ActionGroup.cpp
    src/Command.cpp
    src/CmderFactory.cpp
    src/Vehicle.cpp
)

add_library(executor STATIC ${EXECUTOR_SOURCES})
target_include_directories(executor PUBLIC ${INCLUDE_DIRS})

# 主程序路径修改
add_executable(smartcar_main src/main.cpp)
target_link_libraries(smartcar_main PRIVATE executor)

# GoogleTest via FetchContent
include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 合并所有测试到同一个可执行文件
add_executable(ExecutorTest
    tests/BaseTests.cpp
    tests/StateTests.cpp
    tests/SequenceTests.cpp
    tests/TurnRoundTests.cpp
)
target_link_libraries(ExecutorTest PRIVATE executor GTest::gtest_main)

# 使用 gtest_discover_tests 让 CTest 识别每个 TEST 用例为独立测试条目
include(GoogleTest)
gtest_discover_tests(ExecutorTest
    TEST_PREFIX "UT."
    NO_PRETTY_TYPES
    NO_PRETTY_VALUES
    DISCOVERY_TIMEOUT 30
)