cmake_minimum_required(VERSION 3.2)

project(ComponentManager CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_ERROR_DEPRECATED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 COMPONENTS
             Core
             Gui)

# ==============================================================================

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

# ==============================================================================

get_filename_component(PROJECT_DIR "." ABSOLUTE)


# Install directories relative to install prefix

set(INSTALL_BINARY_DIR  bin)
set(INSTALL_INCLUDE_DIR include)
set(INSTALL_LIBRARY_DIR lib)
#set(INSTALL_SOURCE_DIR  source)
set(INSTALL_EXAMPLE_DIR examples)

if (WIN32)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR share/cmake/ComponentManager)
endif ()

#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Targets

# ==============================================================================
# ==============================================================================

# Lib sources
add_subdirectory("${PROJECT_DIR}/source" ./lib)

# Thirdparty
add_subdirectory("${PROJECT_DIR}/thirdparty")

# Tests
add_subdirectory("${PROJECT_DIR}/tests" ./bin)

# Examples
add_subdirectory("${PROJECT_DIR}/examples" ./examples)

# Docs
#add_subdirectory("${PROJECT_DIR}/doc" ./doc)

# vim:ft=cmake:fenc=utf-8:ff=unix:ts=2:sw=2:tw=80:et:
