cmake_minimum_required(VERSION 3.21)
project("search_engine" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(JSON BuildTest OFF CACHE INTERNAL "")


set(MY_CLASSES_DIR ${CMAKE_SOURCE_DIR}/classes)
set(MY_DATA_DIR ${CMAKE_SOURCE_DIR}/data)
set(MY_TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)



include(FetchContent) 
FetchContent_Declare(
googletest
URL
https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)


include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest) 
enable_testing()

include(GoogleTest)


add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)
