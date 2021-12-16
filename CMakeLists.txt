cmake_minimum_required(VERSION 3.13)
project(komiwojazer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion -Wunused-variable)

include_directories(include)

set(SOURCE_FILES
        src/tsp_setup.cpp
        src/TSP.cpp
        )

set(SOURCES_FILES_TESTS
        tests/komiwojazer_tests.cpp
        )

add_executable(tsp ${SOURCE_FILES} main.cpp)

add_executable(komiwojazer_tests ${SOURCE_FILES} ${SOURCE_FILES_TEST} tests/main_gtest.cpp)

target_include_directories(komiwojazer_tests PUBLIC
        D:/Programowanie/googletest-master/googlemock/include
        D:/Programowanie/googletest-master/googletest/include)

add_subdirectory(D:/Programowanie/googletest-master googletest-master)

target_link_libraries(komiwojazer_tests gmock)