cmake_minimum_required(VERSION 3.20)
project(ring_buffer_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

# Fetch GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(singlethread_tests single_threaded_ring_buffer_tests.cpp)
target_link_libraries(singlethread_tests GTest::gtest_main GTest::gtest)

add_executable(multithread_tests single_producer_single_customer_tests.cpp
                                 multiple_producers_single_customer_tests.cpp
                                 multiple_producers_multiple_customer_tests.cpp)
target_link_libraries(multithread_tests GTest::gtest_main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(singlethread_tests)
gtest_discover_tests(multithread_tests)