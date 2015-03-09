cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
create_test_sourcelist(TEST_SRCS testdriver.cpp ${SRCS})
add_executable(testdriver ${TEST_SRCS})

foreach(TEST_SRC ${SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_test(NAME ${TEST_NAME} COMMAND testdriver ${TEST_NAME})
endforeach()

find_package(Qt5Core REQUIRED)
target_link_libraries(testdriver Qt5::Core)
target_include_directories(testdriver PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")
