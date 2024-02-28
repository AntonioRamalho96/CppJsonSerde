cmake_minimum_required(VERSION 3.10)

project(JsonSerde)

include_directories(src rapidjson/include)
add_compile_options(-fPIC)
add_library(${PROJECT_NAME} src/Property.cpp
    src/JsonSerde.cpp)

# Testing
enable_testing()

# Import gtest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

link_libraries(${PROJECT_NAME} GTest::gtest_main)
include(GoogleTest)

add_executable(basic_test test/basic_test)
gtest_discover_tests(basic_test)

add_executable(generic_vector_test test/generic_vector_test.cpp)
gtest_discover_tests(generic_vector_test)

add_executable(test_type test/test_type.cpp)
gtest_discover_tests(test_type)

add_executable(schema_validation_test test/schema_validation_test.cpp)
gtest_discover_tests(schema_validation_test)