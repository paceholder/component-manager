cmake_minimum_required(VERSION 3.2)

project(SimpleMainWindow CXX)

set(SIMPLE_MAIN_WINDOW_DIR ${INSTALL_EXAMPLE_DIR}/SimpleMainWindow)

set(INSTALL_BINARY_DIR ${SIMPLE_MAIN_WINDOW_DIR}/bin)
set(INSTALL_LIBRARY_DIR ${SIMPLE_MAIN_WINDOW_DIR}/lib)
set(INSTALL_RESOURCE_DIR ${SIMPLE_MAIN_WINDOW_DIR}/resource)

find_package(Qt5 COMPONENTS Core Gui Widgets)

############################################################

file(GLOB_RECURSE APP_CPP_FILES "App/*.cpp")


add_executable(SimpleMainWindow ${APP_CPP_FILES})

target_include_directories(SimpleMainWindow PRIVATE "CoreComponent")

target_link_libraries(SimpleMainWindow PRIVATE
                      ComponentManager
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets)

# Installation

install(TARGETS SimpleMainWindow
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR})


############################################################

file(GLOB_RECURSE CORE_CPP_FILES "CoreComponent/*.cpp")

add_library(CoreComponent SHARED ${CORE_CPP_FILES})

set_target_properties(CoreComponent PROPERTIES
                      # DEFINE_SYMBOL CORE_SHARED
                      DEFINE_SYMBOL CORE_EXPORTS
                      PREFIX        "")

target_link_libraries(CoreComponent PRIVATE
                      ComponentManager
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets)

# Installation

install(TARGETS CoreComponent
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR})

############################################################

file(GLOB_RECURSE ADDITION_CPP_FILES "AdditionComponent/*.cpp")

add_library(AdditionComponent SHARED ${ADDITION_CPP_FILES})

set_target_properties(AdditionComponent PROPERTIES
                      # DEFINE_SYMBOL ADDITION_SHARED
                      DEFINE_SYMBOL ADDITION_EXPORTS
                      PREFIX        "")

target_link_libraries(AdditionComponent PRIVATE
                      ComponentManager
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets)

# Installation

install(TARGETS AdditionComponent
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR})

install(FILES resource/Addition.json
        DESTINATION ${INSTALL_RESOURCE_DIR}
        RENAME Addition.json)

install(FILES resource/Core.json
        DESTINATION ${INSTALL_RESOURCE_DIR}
        RENAME Core.json)

