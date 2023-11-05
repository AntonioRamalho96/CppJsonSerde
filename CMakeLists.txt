cmake_minimum_required(VERSION 3.10)

project(JsonSerde)

include_directories(src rapidjson/include)
add_library(${PROJECT_NAME} src/JsonSerdeStatics.cpp)

link_libraries(${PROJECT_NAME})
add_executable(basic_test test/basic_test)