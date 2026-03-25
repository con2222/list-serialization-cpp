cmake_minimum_required(VERSION 3.1.2...4.2.3)

set(PROJECT_NAME list-serialization)

project(${PROJECT_NAME} LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE  Release)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/List.cpp
    src/ArgParser.cpp
    src/FileParser.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(list_tests
        tests/test_list.cpp
        src/List.cpp
)

target_include_directories(list_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(list_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(list_tests)