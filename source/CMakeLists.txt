file(GLOB_RECURSE CPP_FILES "${PROJECT_DIR}/source/*.cpp")

add_library(ComponentManager SHARED ${CPP_FILES})

set(CM_VERSION_MAJOR 0)
set(CM_VERSION_MINOR 0)
set(CM_VERSION_PATCH 1)
set(CM_VERSION ${CM_VERSION_MAJOR}.${CM_VERSION_MINOR}.${CM_VERSION_PATCH} )

set_target_properties(ComponentManager PROPERTIES VERSION ${CM_VERSION}
                                                  SOVERSION ${CM_VERSION_MAJOR} )

target_include_directories(ComponentManager PUBLIC "${PROJECT_DIR}/include")
target_include_directories(ComponentManager PRIVATE "${PROJECT_DIR}/include/ComponentManager")
target_include_directories(ComponentManager PRIVATE "${PROJECT_DIR}/source")

target_compile_definitions(ComponentManager PUBLIC "-DCM_SHARED")
target_compile_definitions(ComponentManager PRIVATE "-DCM_EXPORTS")

# It is possible to use DEFINE_SYMBOL just once
set_target_properties(ComponentManager PROPERTIES
                      #DEFINE_SYMBOL CM_EXPORTS
                      PREFIX        "")


target_link_libraries(ComponentManager Qt5::Core)

install(DIRECTORY "${PROJECT_DIR}/include/ComponentManager"
        DESTINATION ${INSTALL_INCLUDE_DIR})

#install(DIRECTORY "${PROJECT_DIR}/source/"
        #DESTINATION ${INSTALL_SOURCE_DIR}
        #PATTERN *.cpp EXCLUDE)

install(TARGETS ComponentManager
        ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        RUNTIME DESTINATION ${INSTALL_BINARY_DIR})

install(FILES "${PROJECT_DIR}/cmake/ComponentManagerConfig.cmake"
        DESTINATION ${INSTALL_CMAKE_DIR})
