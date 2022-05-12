cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VL)

message("=== VL project CMake build configuration process started ===")

option(VL_JSON "Connect JSON support infrastructure (JSONConverter)" OFF)
option(VL_TESTS "Include Test target" OFF)

#message("VL CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if ("${DEPS}" STREQUAL "")
	message("VL: WARNING: DEPS is not specified. Take the default value")
	set(DEPS "${PROJECT_SOURCE_DIR}/dependencies")
else()
	message("VL: DEPS: ${DEPS}")
endif()

set(UTILS_DIR "${DEPS}/Utils/include")
foreach(items ${UTILS_DIR})
	file(GLOB_RECURSE UTILS_SRC "${items}/*.cpp")
endforeach()

set(VL_INCLUDES
	"${PROJECT_SOURCE_DIR}/include"
	"${UTILS_DIR}"
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