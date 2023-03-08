cmake_minimum_required(VERSION 3.21)
# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(JSONConverter C CXX)

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		# "/std:c++latest"
	)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("=== JSONConverter project CMake build configuration process started ===")

option(JSON_TESTS "Add JSONTest target" OFF)

file(STRINGS build_config.sh build_config REGEX ".*extraArg=\"(.*)\"")
message("buid_config: ${build_config}")

# set deps variables
set(JSONConverter_DEPS "$ENV{JSONConverter_deps}")
set(JSONConverter_DEPS "${JSONConverter_DEPS}" PARENT_SCOPE)
if ("${JSONConverter_DEPS}" STREQUAL "")
	set(DEPS "$ENV{build_deps}")
endif()
if ("${DEPS}" STREQUAL "")
	set(DEPS ${JSONConverter_DEPS})
endif()
if ("${DEPS}" STREQUAL "")
	message(SEND_ERROR "JSONConverter: ERROR: DEPS is not specified. Take the default value")
	set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
else()
	message("JSONConverter: DEPS: ${DEPS}")
endif()
set(DEPS ${DEPS} PARENT_SCOPE)

# Build folder name
if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else ()
	set(BUILD_DIR_NAME "Build-cmake")
endif ()

# VL dependency
if ("${VL_DIR}" STREQUAL "")
	set(VL_DIR "${PROJECT_SOURCE_DIR}/../")
else()
	message("VL_DIR: ${VL_DIR}")
endif()
add_subdirectory("${VL_DIR}" "${VL_DIR}/${BUILD_DIR_NAME}")

#rapidjson dependency directory
if ("${RAPIDJSON_DIR}" STREQUAL "")
	set(RAPIDJSON_DIR "${DEPS}/rapidjson")
else()
	message("RAPIDJSON_DIR: ${RAPIDJSON_DIR}")
endif()

# includes
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