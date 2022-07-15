cmake_minimum_required(VERSION 3.21)
# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(JSONConverter)

message("=== JSONConverter project CMake build configuration process started ===")

option(JSON_TESTS "Add JSONTest target" OFF)

file(STRINGS build_config.sh build_config REGEX ".*extraArg=\"(.*)\"")
message("buid_config: ${build_config}")

if ("${DEPS}" STREQUAL "")
	message("WARNING: DEPS is not specified. Take the default value")
	set(DEPS "${PROJECT_SOURCE_DIR}/dependencies")
else()
	message("DEPS: ${DEPS}")
endif()

if ("${VL_DIR}" STREQUAL "")
	set(VL_DIR "${PROJECT_SOURCE_DIR}/../")
else()
	message("VL_DIR: ${VL_DIR}")
endif()

if ("${RAPIDJSON_DIR}" STREQUAL "")
	set(RAPIDJSON_DIR "${DEPS}/rapidjson")
else()
	message("RAPIDJSON_DIR: ${RAPIDJSON_DIR}")
endif()

#message("JSONConverter CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else ()
	set(BUILD_DIR_NAME "Build-cmake")
endif ()

add_subdirectory("${VL_DIR}" "${VL_DIR}/${BUILD_DIR_NAME}")

set(JSONCONVERTER_INCLUDES
	"${VL_INCLUDES}"
	"${RAPIDJSON_DIR}/include"
	"${PROJECT_SOURCE_DIR}/include"
)
set(JSONCONVERTER_INCLUDES ${JSONCONVERTER_INCLUDES} PARENT_SCOPE)

message("JSONCONVERTER_INCLUDES: ${JSONCONVERTER_INCLUDES}")
include_directories(${PROJECT_BINARY_DIR})
include_directories("${JSONCONVERTER_INCLUDES}")

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

# add_custom_command(
#     TARGET JSONConverter PRE_BUILD
#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
#     VERBATIM
# )