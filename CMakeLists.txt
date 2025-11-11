cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(threat-monitor VERSION 0.1.0 LANGUAGES CXX C)

if(WIN32)
    message(FATAL_ERROR "Unsupported platform")
endif()


add_subdirectory(agent)
add_subdirectory(server)
add_subdirectory(monitor)

include(CTest)
enable_testing()
add_subdirectory(tests)