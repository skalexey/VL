cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

message("JSONConverter CMakeLists")

project(JSONConverter)
option(JSON_TESTS "Add JSONTest target" OFF)

include_directories(${PROJECT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/VL/include")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/VL")

message("JSONConverter added VL")

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
