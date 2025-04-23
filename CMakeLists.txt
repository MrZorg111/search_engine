cmake_minimum_required(VERSION 3.21)
project("search_engine" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MY_RES_DIR ${CMAKE_SOURCE_DIR}/resources)


include(FetchContent) 
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest) 


include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(JSON BuildTest OFF CACHE INTERNAL "")

enable_testing()

add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)
