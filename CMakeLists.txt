project(qjsonmapper)
cmake_minimum_required(VERSION 2.8.11)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
