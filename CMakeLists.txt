#######################################################################################
## liblog CMake Project File
##
## This simple project file makes it easy and clean to include libut in CMake projects
## by either including the repo as git submodule or cloning it into the project tree.
## The resulting folder can be directly used as subproject and defines a variable that
## contains the include directories.
##
## Usage:
## -------
##
## In main project file, before any subprojects that depend on liblog:
##    add_subdirectory(log)
##
## In each subproject that uses libcl:
##    include_directories(${LIBLOG_INCLUDE_DIRS})
##
## Linking to the supplied libraries is needed:
##	  target_link_libraries(<target> ${LIBLOG_LIBRARIES})
##
##

# Version requirement and project info
cmake_minimum_required(VERSION 3.1.3)
project(log)

# C++ Standard
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


# Set user variables
set(LIBLOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")
set(LIBLOG_LIBRARIES log CACHE INTERNAL "")

# Only include ut target if it isnt already included by something else
if (NOT TARGET ut)
	add_subdirectory(ut)
endif()

# Add libut and own include directories to allow own source files to import headers more easily
include_directories(${LIBUT_INCLUDE_DIRS} ${LIBLOG_INCLUDE_DIRS}/log)

# Set source files
set(SOURCE_FILES
		src/default_formatter.cxx
		src/log_entry.cxx
		src/logger.cxx
)

# Build library
add_library(log ${SOURCE_FILES})

# Require support for at least C++14.
set_property(TARGET log PROPERTY CXX_STANDARD 14)
set_property(TARGET log PROPERTY CXX_STANDARD_REQUIRED ON)
