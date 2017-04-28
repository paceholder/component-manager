if (CMAKE_VERSION VERSION_LESS 2.8.3)
  message(FATAL_ERROR "ComponentManager requires at least CMake version 2.8.3")
endif()

if (WIN32)
  get_filename_component(_ComponentManager_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)
else()
  get_filename_component(_ComponentManager_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
endif ()

set(ComponentManager_VERSION_STRING 0.1.0)

macro(_dependency_manager_check_file_exists file Target)
  if(NOT EXISTS "${file}" )
    message(FATAL_ERROR "The imported target \"${Target}\" references the file
    \"${file}\"
    but this file does not exist.  Possible reasons include:
    * The file was deleted, renamed, or moved to another location.
    * An install or uninstall procedure did not complete successfully.
    * The installation package was faulty and contained
    \"${CMAKE_CURRENT_LIST_FILE}\"
    but not all the files it references.")
  endif()
endmacro()

macro(_populate_dependency_manager_target_properties Target Configuration Dependencies LIB_LOCATION IMPLIB_LOCATION)
  set_property(TARGET ${Target} APPEND PROPERTY IMPORTED_CONFIGURATIONS ${Configuration})
  _dependency_manager_check_file_exists(${LIB_LOCATION} ${Target})
  set_target_properties(${Target} PROPERTIES
    "IMPORTED_LINK_INTERFACE_LIBRARIES_${Configuration}" "${Dependencies}"
    "IMPORTED_LOCATION_${Configuration}" ${LIB_LOCATION}
    )
  if(NOT "${IMPLIB_LOCATION}" STREQUAL "")
    _dependency_manager_check_file_exists(${IMPLIB_LOCATION} ${Target})
    set_target_properties(${Target} PROPERTIES
      "IMPORTED_IMPLIB_${Configuration}" ${IMPLIB_LOCATION}
    )
  endif()
endmacro()

if (NOT TARGET ComponentManager)
  set(ComponentManager_FOUND False)

  find_package(Uni REQUIRED)

  add_library(ComponentManager SHARED IMPORTED)

  set(ComponentManager_LIBRARY_DIR "${_ComponentManager_INSTALL_PREFIX}/lib")
  set(ComponentManager_BINARY_DIR "${_ComponentManager_INSTALL_PREFIX}/bin")

  if(UNIX)
    #set(_temp_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    #set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    #find_library(ComponentManager_IMPRT_LIBRARY
    #           NAMES ComponentManager
    #           PATHS ${ComponentManager_LIBRARY_DIR})
    #set(CMAKE_FIND_LIBRARY_SUFFIXES ${_temp_CMAKE_FIND_LIBRARY_SUFFIXES})
    find_library(ComponentManager_LIBRARY
             NAMES ComponentManager
             PATHS ${ComponentManager_LIBRARY_DIR})
    _populate_dependency_manager_target_properties(ComponentManager RELEASE "" ${ComponentManager_LIBRARY} "")
    _populate_dependency_manager_target_properties(ComponentManager DEBUG "" ${ComponentManager_LIBRARY} "")
  else()
    set(_temp_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a")
    find_library(ComponentManager_IMPRT_LIBRARY
               NAMES ComponentManager
               PATHS ${ComponentManager_LIBRARY_DIR})
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${_temp_CMAKE_FIND_LIBRARY_SUFFIXES})
    find_library(ComponentManager_LIBRARY
             NAMES ComponentManager
             PATHS ${ComponentManager_BINARY_DIR})
    _populate_dependency_manager_target_properties(ComponentManager RELEASE "" ${ComponentManager_LIBRARY} ${ComponentManager_IMPRT_LIBRARY})
    _populate_dependency_manager_target_properties(ComponentManager DEBUG "" ${ComponentManager_LIBRARY} ${ComponentManager_IMPRT_LIBRARY})
  endif()

  list(APPEND ComponentManager_INCLUDE_DIRS "${_ComponentManager_INSTALL_PREFIX}/include")
  list(APPEND ComponentManager_INCLUDE_DIRS "${Loki_INCLUDE_DIRS}")

  _dependency_manager_check_file_exists(${ComponentManager_INCLUDE_DIRS} ComponentManager)

  set_property(TARGET ComponentManager PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ComponentManager_INCLUDE_DIRS})

  set(ComponentManager_DEFENITIONS "ComponentManager_SHARED ${Loki_DEFENITIONS}")

  set_property(TARGET ComponentManager PROPERTY INTERFACE_COMPILE_DEFINITIONS ${ComponentManager_DEFENITIONS})

  set(ComponentManager_FOUND True)

  message(STATUS "Found ComponentManager Library: ${ComponentManager_IMPRT_LIBRARY} ${ComponentManager_LIBRARY}")
endif()
