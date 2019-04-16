project(ComPairDAQ CXX)
cmake_minimum_required(VERSION 3.0)

#We require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if user does not specify prefix we assign it to a default value.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		message(STATUS "Install Prefix not specified.")
		file(MAKE_DIRECTORY install)
		get_filename_component(INSTALL_DIR ${CMAKE_SOURCE_DIR}/install REALPATH)
		set(CMAKE_INSTALL_PREFIX ${INSTALL_DIR} CACHE PATH "Install Prefix" FORCE)
endif()
message(STATUS "Installing to ${CMAKE_INSTALL_PREFIX}")

include_directories(include)
add_subdirectory(source)

