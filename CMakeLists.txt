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

option(tests "Enable tests" OFF)

#Treat warnings as errors for testing.
option(WARNINGS_AS_ERRORS "Treat all warnings as errors." OFF)

if (WARNINGS_AS_ERRORS)
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		list(APPEND CMAKE_CXX_FLAGS "-Wall -Wpedantic -Werror")
	elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		list(APPEND CMAKE_CXX_FLAGS "-Werror")
	endif ()
	message(STATUS "Warnings are being treated as errors.")
endif(WARNINGS_AS_ERRORS)

#Begin processing code

include_directories(include)
add_subdirectory(source)

if(${tests})
	enable_testing()
	add_subdirectory(Catch)
	add_subdirectory(tests)
endif(${tests})

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

