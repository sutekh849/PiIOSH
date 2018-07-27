cmake_minimum_required(VERSION 3.1)
project(arduinocontroller)
set (CMAKE_CXX_STANDARD 11)
find_package(firmatacpp REQUIRED)
find_package (serial REQUIRED)
set(IOSH_SOURCES
	arduinocontrol.cpp
	main.cpp
	netthread.cpp
	udp_server.cpp
)
set(IOSH_INCLUDES
	arduinocontrol.h
	netthread.h
	udp_server.h
	)
add_subdirectory(boost-cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
#link_directories(${Boost_LIBRARY_DIRS})
add_executable(qtIOSH WIN32 ${IOSH_SOURCES} ${IOSH_INCLUDES} )
if(WIN32)
    target_link_libraries (qtIOSH PUBLIC  ws2_32 firmatacpp serial Boost::system Boost::boost Boost::exception )
    ELSE()
    target_link_libraries (qtIOSH PUBLIC firmatacpp serial Boost::system Boost::boost Boost::exception )
ENDIF()
