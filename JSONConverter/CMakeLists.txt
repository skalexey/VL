cmake_minimum_required (VERSION 3.15)

project(JSONConverter C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")

	option(JSON_TESTS "Add JSONTest target" OFF)

	set(VL_DIR "${PROJECT_SOURCE_DIR}/../")
	add_dependency_module(${VL_DIR} VL)

	#rapidjson dependency directory
	set(RAPIDJSON_DIR "${DEPS}/rapidjson")
	module_add_includes(${RAPIDJSON_DIR}/include)

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
	
	add_headers_to_sources()
	
	# add library
	module_add_library()
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
