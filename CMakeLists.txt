# cmake_minimum_required(VERSION <specify CMake version here>)
project(Projekt__TS_2_serwer)

set(CMAKE_CXX_STANDARD 17)

add_executable(Serwer serwer.cpp)
add_executable(Klient klient.cpp)

if(WIN32)
    target_link_libraries(Serwer wsock32 ws2_32)
    target_link_libraries(Klient wsock32 ws2_32)
endif()