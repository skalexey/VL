#pragma once
#ifdef LOG_ON
	#include <iostream>
	#define LOG(msg) std::cout << msg
	#define LOG_ERROR(msg) std::cout << "Error! " << msg << "\n"
	#define LOG_WARNING(msg) std::cout << "Warning: " << msg << "\n"
	#define LOG_INFO(msg) std::cout << msg << "\n"
#else
	#define LOG(msg)
	#define LOG_WARNING(msg)
	#define LOG_ERROR(msg)
	#define LOG_INFO(msg)
#endif

