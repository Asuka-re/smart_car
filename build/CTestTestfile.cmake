# CMake generated Testfile for 
# Source directory: C:/smart_car
# Build directory: C:/smart_car/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ExecutorTest "C:/smart_car/build/ExecutorTest.exe")
set_tests_properties(ExecutorTest PROPERTIES  _BACKTRACE_TRIPLES "C:/smart_car/CMakeLists.txt;31;add_test;C:/smart_car/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
