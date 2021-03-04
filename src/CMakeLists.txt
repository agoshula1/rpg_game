# based on https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/src/CMakeLists.txt

# get list of header files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${RPG_SOURCE_DIR}/include/rpg/*.hpp")

# get list of source files
file(GLOB SRC_LIST CONFIGURE_DEPENDS "${RPG_SOURCE_DIR}/src/*.cpp")

# Make automatic libary - will be static or dynamic based on user setting
add_library(rpg_library ${SRC_LIST} ${HEADER_LIST})

# include required directory
target_include_directories(rpg_library PUBLIC "${RPG_SOURCE_DIR}/include")

#IDEs should put the headers in a nice place
source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

#IDEs place source files in a nice place(?)
source_group(
    TREE "${PROJECT_SOURCE_DIR}/source"
    PREFIX "Source Files"
    FILES ${SRC_FILES})