cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(JSONConverter)
option(JSON_TESTS "Add JSONTest target" OFF)

set(DEPS "dependencies")
set(VL_DIR "${PROJECT_SOURCE_DIR}/../")

include_directories(${PROJECT_BINARY_DIR})
include_directories("${VL_DIR}/include")
include_directories("${DEPS}/rapidjson/include")
include_directories("${PROJECT_SOURCE_DIR}/include")

if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(VL_BUILD_DIR_NAME "Build-g++-cmake-${CMAKE_BUILD_TYPE}")
else ()
	set(VL_BUILD_DIR_NAME "Build-cmake-${CMAKE_BUILD_TYPE}")
endif ()

add_subdirectory("${VL_DIR}" "${VL_DIR}/${VL_BUILD_DIR_NAME}")

file(GLOB SRC "src/*")
file(GLOB HEADERS "include/*")
add_library(JSONConverter ${SRC} ${HEADERS})

target_link_libraries (JSONConverter VL)

configure_file(
	"${PROJECT_SOURCE_DIR}/JSONConverterConfig.h.in"
	"${PROJECT_BINARY_DIR}/JSONConverterConfig.h"
)

if (JSON_TESTS)
	add_executable(JSONTest test.cpp)
	target_link_libraries(JSONTest JSONConverter)
endif ()
