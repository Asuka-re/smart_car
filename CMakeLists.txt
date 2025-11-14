cmake_minimum_required(VERSION 3.14)
project(smart_car LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. 先建库
add_library(executor STATIC src/executor.cpp)
target_include_directories(executor PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 2. 主程序
add_executable(executor_app src/main.cpp)
target_link_libraries(executor_app PRIVATE executor)

# 3. GoogleTest 检测
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/googletest/CMakeLists.txt")
  add_subdirectory(third_party/googletest)
  set(GTEST_AVAILABLE TRUE)
else()
  set(GTEST_AVAILABLE FALSE)
endif()

# 4. 测试可执行文件
if(GTEST_AVAILABLE)
  add_executable(ExecutorTest tests/ExecutorTest.cpp)
  target_include_directories(ExecutorTest PRIVATE
                             ${CMAKE_SOURCE_DIR}/include
                             ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)
  target_link_libraries(ExecutorTest PRIVATE executor GTest::gtest_main)
  add_dependencies(ExecutorTest executor)
  enable_testing()
  add_test(NAME ExecutorTest COMMAND ExecutorTest)
else()
  add_executable(ExecutorTest_simple tests/ExecutorTest_simple.cpp)
  target_include_directories(ExecutorTest_simple PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(ExecutorTest_simple PRIVATE executor)
  enable_testing()
  add_test(NAME ExecutorTest_simple COMMAND ExecutorTest_simple)
endif()

# 5. 统一输出（仅对已存在目标）
set(output_targets executor executor_app)
if(GTEST_AVAILABLE)
  list(APPEND output_targets ExecutorTest)
else()
  list(APPEND output_targets ExecutorTest_simple)
endif()
set_target_properties(${output_targets}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})