#pragma once

//#define LOG_LEVEL_VERBOSE

#ifdef LOG_ON
	#include <iostream>
	#define LOG(msg) std::cout << msg
	#define LOG_ERROR(msg) std::cout << "Error! " << msg << "\n"
	#define LOG_WARNING(msg) std::cout << "Warning: " << msg << "\n"
	#define LOG_INFO(msg) std::cout << msg << "\n"
#ifdef LOG_LEVEL_VERBOSE
	#define LOG_VERBOSE(msg) std::cout << "[V] " << msg << "\n"
#else
	#define LOG_VERBOSE(msg)
#endif
#else
	#define LOG(msg)
	#define LOG_WARNING(msg)
	#define LOG_ERROR(msg)
	#define LOG_INFO(msg)
	#define LOG_VERBOSE(msg)
#endif

