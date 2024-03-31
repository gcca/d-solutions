add_executable(d-solutions_test d-solutions_test.cc dsolutions.cpp)
target_link_libraries(d-solutions_test GTest::gtest_main)
gtest_discover_tests(d-solutions_test)
