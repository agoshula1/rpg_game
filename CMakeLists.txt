# based on https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/CMakeLists.txt
# works with CMake 3.19
cmake_minimum_required(VERSION 3.19)

# set the project name, version, languages
project(
    RPG
    VERSION 1.0
    LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # testing only available if this is the main app
    include(CTest)
endif()

# add directory with library code
add_subdirectory(src)

# add directory with executable code
add_subdirectory(app)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(test)
endif()
