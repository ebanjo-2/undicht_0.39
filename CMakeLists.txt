# the version i am currently using (required version may be much lower)
cmake_minimum_required(VERSION "3.1.0")
set (CMAKE_CXX_STANDARD 11)

# the name of the project described in this file
project("undicht")

# adding engine libraries
add_subdirectory("core")
add_subdirectory("graphics")
add_subdirectory("tools")


# adding example applications
add_subdirectory("examples/Hello World")