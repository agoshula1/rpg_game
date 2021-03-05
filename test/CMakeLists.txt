# based on https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/tests/CMakeLists.txt

# add Boost (to use Boost.Test)
# SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost_1_70_0") # boost installed in a non-default location
# SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost_1_70_0/libs")
find_package(Boost)

# add each test file as executable
add_executable(testchar CharacterTest.cpp)

add_executable(testmove MoveTest.cpp)
# TODO add tests for other library classes

# link each test executable to main library and Boost
target_link_libraries(testchar PRIVATE rpg_library Boost::boost)

target_link_libraries(testmove PRIVATE rpg_library Boost::boost)

# TODO link libraries to other test executables

# register tests
add_test(NAME testchar COMMAND testchar)

add_test(NAME testmove COMMAND testmove)

# TODO register other tests
