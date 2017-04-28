file(GLOB_RECURSE TEST_CPP_FILES "*.cpp")

add_executable(ComponentManagerTests ${TEST_CPP_FILES})

target_link_libraries(ComponentManagerTests PUBLIC ComponentManager)
target_link_libraries(ComponentManagerTests PUBLIC catch)

add_test(NAME ComponentManagerTests
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         COMMAND ComponentManagerTests)
