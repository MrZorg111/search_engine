cmake_minimum_required(VERSION 3.21)

add_executable("search_engine" main.cpp converterjson.cpp invertedindex.cpp searchserver.cpp ${MY_TESTS_DIR}/test.cpp
                                                                            ${MY_CLASSES_DIR}/converterjson.h ${MY_CLASSES_DIR}/invertedindex.h 
                                                                            ${MY_CLASSES_DIR}/searchserver.h
                                                                            ${MY_DATA_DIR}/answer.json ${MY_DATA_DIR}/config.json ${MY_DATA_DIR}/request.json)
                                        
target_include_directories("search_engine" PUBLIC ${MY_CLASSES_DIR} ${MY_DATA_DIR} ${MY_TESTS_DIR})
target_link_libraries("search_engine" PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries("search_engine" PRIVATE gtest_main)
gtest_discover_tests("search_engine")