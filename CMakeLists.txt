cmake_minimum_required(VERSION 3.21)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(VL C CXX)

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		"/std:c++latest"
	)
else()
	set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("=== VL project CMake build configuration process started ===")

option(VL_JSON "Connect JSON support infrastructure (JSONConverter)" OFF)
option(VL_TESTS "Include Test target" OFF)

#message("VL CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# set deps variables
set(vl_DEPS "$ENV{vl_deps}")
set(vl_DEPS "${vl_DEPS}" PARENT_SCOPE)
if ("${vl_DEPS}" STREQUAL "")
	set(DEPS "$ENV{build_deps}")
endif()
if ("${DEPS}" STREQUAL "")
	set(DEPS ${vl_DEPS})
endif()
if ("${DEPS}" STREQUAL "")
	message(SEND_ERROR "vl: ERROR: DEPS is not specified. Take the default value")
	set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
else()
	message("VL: DEPS: ${DEPS}")
endif()
set(DEPS ${DEPS} PARENT_SCOPE)

# Build directory name
if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else ()
	set(BUILD_DIR_NAME "Build-cmake")
endif ()

# Utils dependency
set(UTILS_DIR "${DEPS}/Utils")
add_subdirectory ("${UTILS_DIR}" "${UTILS_DIR}/${BUILD_DIR_NAME}")
set(CUSTOM_INCLUDES ${CUSTOM_INCLUDES} ${utils_INCLUDES})

# includes
set(VL_INCLUDES
	"${PROJECT_SOURCE_DIR}/include"
	"${CUSTOM_INCLUDES}"
)

if (NOT "${INCLUDES}" STREQUAL "")
	message("VL: External INCLUDES variable: ${INCLUDES}")
	set(VL_INCLUDES ${VL_INCLUDES} ${INCLUDES})
	foreach(items ${INCLUDES})
		file(GLOB_RECURSE INCLUDES_SRC "${items}/*.cpp")
	endforeach()
	message("VL: External INCLUDES sources: ${INCLUDES_SRC}")
endif()

set(VL_INCLUDES ${VL_INCLUDES} PARENT_SCOPE)
message("VL: VL_INCLUDES: ${VL_INCLUDES}")

include_directories("${VL_INCLUDES}")
include_directories(${PROJECT_BINARY_DIR})

file(GLOB SRC "src/*")
set(SRC ${SRC} ${INCLUDES_SRC} ${UTILS_SRC})

file(GLOB HEADERS "include/*")

message("VL: HEADERS: ${HEADERS}")
message("VL: SRC: ${SRC}")

add_library(VL ${SRC} ${HEADERS})
target_link_libraries(VL utils)

message("=== VL library added ===")

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

# add_custom_command(
#     TARGET VL PRE_BUILD
#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
#     VERBATIM
# )