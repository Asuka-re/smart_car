cmake_minimum_required(VERSION 3.14)
project(smart_car LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(executor STATIC src/executor.cpp)
target_include_directories(executor PUBLIC ${CMAKE_SOURCE_DIR}/include)

# 输出到顶层 build 目录，方便直接运行
set_target_properties(executor PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(executor_app src/main.cpp)
target_link_libraries(executor_app PRIVATE executor)
set_target_properties(executor_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 优先使用本地 vendored googletest；若不存在，直接使用离线简单测试 runner（不再尝试在线下载）
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/googletest/CMakeLists.txt")
  message(STATUS "Using vendored googletest in third_party/googletest")
  add_subdirectory(third_party/googletest)
  set(GTEST_AVAILABLE TRUE)
else()
  message(STATUS "Vendored googletest NOT found; will build simple test runner (no googletest).")
  set(GTEST_AVAILABLE FALSE)
endif()

if(GTEST_AVAILABLE)
  add_executable(ExecutorTest tests/ExecutorTest.cpp)
  target_include_directories(ExecutorTest PRIVATE ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include
  )
  target_link_libraries(ExecutorTest PRIVATE executor GTest::gtest_main)
  set_target_properties(ExecutorTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  enable_testing()
  add_test(NAME ExecutorTest COMMAND ${CMAKE_BINARY_DIR}/ExecutorTest.exe)
else()
  add_executable(ExecutorTest_simple tests/ExecutorTest_simple.cpp)
  target_include_directories(ExecutorTest_simple PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(ExecutorTest_simple PRIVATE executor)
  set_target_properties(ExecutorTest_simple PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  enable_testing()
  add_test(NAME ExecutorTest_simple COMMAND ${CMAKE_BINARY_DIR}/ExecutorTest_simple.exe)
endif()