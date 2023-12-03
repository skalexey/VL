cmake_minimum_required (VERSION 3.15)

project(VL C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	module_message("Parent dir: ${PARENT_DIR}")

	option(VL_JSON "Connect JSON support infrastructure (JSONConverter)" OFF)
	option(VL_TESTS "Include Test target" OFF)

	# Utils dependency
	set(UTILS_DIR "${DEPS}/Utils")
	module_add_includes(${UTILS_DIR}/include)

	include_directories(${PROJECT_BINARY_DIR})
	add_src( 
		${UTILS_DIR}/src/string_utils.cpp
	)
	if (VL_JSON)
		add_dependency_module(${PROJECT_SOURCE_DIR}/JSONConverter JSONConverter)
	endif ()

	if (VL_TESTS)
		add_executable(VLTest test.cpp)
		target_link_libraries(VLTest VL)
	endif ()

	# Group sources for IDE
	add_headers_to_sources()
	group_headers()

	# add library
	module_add_library()

	# add_custom_command(
	#     TARGET VL PRE_BUILD
	#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
	#     VERBATIM
	# )
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
