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


project(boost_filesystem)
string(APPEND CMAKE_CXX_FLAGS " -lpthread")
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")
string(APPEND CMAKE_CXX_FLAGS " -DBOOST_ALL_DYN_LINK")
add_definitions(-DBOOST_LOG_DYN_LINK) 

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_executable(main
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_compile_options(main INTERFACE -DBOOST_LOG_DYN_LINK)

target_include_directories(main
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

hunter_add_package(Boost COMPONENTS system log thread filesystem date_time chrono)
find_package(Boost CONFIG REQUIRED system thread log filesystem date_time chrono)

list(APPEND Boost_LIBRARIES boost_log_setup)

target_link_libraries(main Boost::log Boost::filesystem Boost::date_time Boost::chrono)

enable_testing()
add_test(NAME unit_tests COMMAND tests) 

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  add_coverage(boost_filesystem)
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
