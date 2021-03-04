# based on https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/apps/CMakeLists.txt

# add game executable
add_executable(game Game.cpp)

# link to main library
target_link_libraries(game PRIVATE rpg_library)