cmake_minimum_required(VERSION 3.6)
find_package(Git)
project(csv2xls)
enable_testing()
set(VERSION_UPDATE_FROM_GIT "TRUE")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
#
# Make a version file containing the current version from git.
#
include(GetVersionFromGitTag)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/version.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")



include_directories("${PROJECT_BINARY_DIR}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/csv2xlslib)
set(CMAKE_CXX_STANDARD 11)
add_subdirectory(csv2xlslib)
add_subdirectory(csv2xlslib.Test)
add_executable(csv2xls csv2xls.cpp ${version_file})
target_link_libraries(csv2xls csv2xlslib xls csv)
add_test( csv2xlslibTest csv2xlslib.Test/csv2xlslibTest.cpp)
