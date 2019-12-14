cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.218.tar.gz"
  SHA1 "9a3594a61227967fb92159339ba326701b287b19"
)

project(SHA)

hunter_add_package(Boost COMPONENTS chrono date_time thread log filesystem system)
find_package (Threads)
find_package (Boost COMPONENTS log_setup)
find_package(Boost CONFIG REQUIRED log_setup chrono date_time thread log filesystem system)


add_library(header INTERFACE)
add_library(Log INTERFACE)
add_library(picosha2 INTERFACE)

add_executable(main
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(header
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(Log
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(picosha2
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


target_link_libraries(Log INTERFACE Boost::log_setup Boost::chrono Boost::date_time Boost::thread Boost::log Boost::filesystem Boost::system)
target_link_libraries(header INTERFACE picosha2 Log ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(main header)

