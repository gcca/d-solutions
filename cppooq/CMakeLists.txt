add_executable(objectpool-test objectpool-test.cc objectpool.cpp)
target_link_libraries(objectpool-test GTest::gtest_main)
gtest_discover_tests(objectpool-test)

add_executable(lru-test lru-test.cc lru.cpp)
target_link_libraries(lru-test GTest::gtest_main)
gtest_discover_tests(lru-test)

add_executable(hashtable-test hashtable-test.cc hashtable.cpp)
target_link_libraries(hashtable-test GTest::gtest_main)
gtest_discover_tests(hashtable-test)
