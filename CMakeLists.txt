# Using target_link_library to link an object library to qt requires 3.12
# 3.12.4 is the maximum version supported on travis / linux out of the box
cmake_minimum_required(VERSION 3.12)

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Using CCache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# Enable the source_group command for creating IDE folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Using C and CXX because GLEW is C
project(qtPlayground CXX)

# Mac OS X specific configuration. In theory must be done before the PROJECT directive, but that doesn't actually work.
# These settings are ignored on other platforms
# We are using deployment target 10.12 because recent Qt versions (e.g. 5.12) have this as
# their minimum, see: https://doc.qt.io/qt-5.12/macos.html
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)

# Compiler detection
set(COMPILER_IS_CLANG FALSE)
set(COMPILER_IS_GNU FALSE)
set(COMPILER_IS_MSVC FALSE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(COMPILER_IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_IS_GNU TRUE)
elseif(MSVC EQUAL 1)
    set(COMPILER_IS_MSVC TRUE)
else()
    message(FATAL_ERROR "Unsupported compiler detected.")
endif()

# include(cmake/Utils.cmake)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(app)
