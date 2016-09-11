#######################################################################################
## liblog CMake Project File
##
## This simple project file makes it easy and clean to include libut in CMake projects
## by either including the repo as git submodule or cloning it into the project tree.
## The resulting folder can be directly used as subproject.
##
## Usage:
## -------
##
## In main project file, before any subprojects that depend on liblog:
##    add_subdirectory(log)
##
## Linking to the supplied libraries is needed:
##	  target_link_libraries(<target> ${LIBLOG_LIBRARIES})
##
##

# Version requirement and project info
cmake_minimum_required(VERSION 3.1.3)
project(log)

# Set user variables
set(LIBLOG_LIBRARIES log CACHE INTERNAL "")

# Add libut project directory.
add_subdirectory(ut)

# Source files
file(GLOB SOURCE_FILES src/*.cxx)

# Build as static library
add_library(log ${SOURCE_FILES})

# Link to libut. This will also add all required include directories.
target_link_libraries(log ${LIBUT_LIBRARIES})

# Link to threading library if on posix or msys
if((NOT WIN32) OR MSYS)
	target_link_libraries(log pthread)
endif()

# Add top include directory as public dependency.
# This automatically adds them to parent projects when linking to libcl.
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	
# Add log subfolder as private dependency.
# It will thus not be available to parent projects.
target_include_directories(log PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/log)

# Require support for at least C++14.
set_property(TARGET log PROPERTY CXX_STANDARD 14)
set_property(TARGET log PROPERTY CXX_STANDARD_REQUIRED ON)
