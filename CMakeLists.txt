cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VL)
option(VL_JSON "Connect JSON support infrastructure (JSONConverter)" OFF)
option(VL_TESTS "Include Test target" OFF)
message("VL CMakeLists")
include_directories("include")
include_directories(${PROJECT_BINARY_DIR})

file(GLOB SRC "src/*")
file(GLOB HEADERS "include/*")
add_library(VL ${SRC} ${HEADERS})
message("VL added library")

if (VL_JSON)
	include_directories(${PROJECT_SOURCE_DIR}/JSONConverter)
	add_subdirectory("JSONConverter")
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
endif ()

if (VL_TESTS)
	add_executable(VLTest test.cpp)
	target_link_libraries(VLTest VL)
	target_link_libraries(VLTest ${EXTRA_LIBS})
endif ()
