#pragma once

//#define LOG_LEVEL_VERBOSE

#ifdef LOG_ON
	#include <iostream>
	#define LOG_TITLE(title) namespace { const char* logTitle = title; }
	#define LOG_STREAM(s) namespace {auto logStream = s;}
	#define SET_LOG_VERBOSE(boolVal) namespace {bool verboseLevel = boolVal;}
	#define LOCAL_LOG(msg) logStream() << "[" << logTitle << "] " << msg
	#define LOCAL_ERROR(msg) logStream() << "[" << logTitle << "] " << "Error! " << msg << "\n"
	#define LOCAL_WARNING(msg) logStream() << "[" << logTitle << "] " << "Warning: " << msg << "\n"
	#define LOCAL_INFO(msg) logStream() << "[" << logTitle << "] " << msg << "\n"
	#define LOCAL_VERBOSE(msg) if (verboseLevel) { logStream() << "[" << logTitle << "] " << "[V] " << msg << "\n"; }
	#ifdef LOG_LEVEL_VERBOSE
		#define LOG_VERBOSE(msg) std::cout << "[V] " << msg << "\n"
	#else
		#define LOG_VERBOSE(msg)
	#endif
	#define LOG(msg) std::cout << msg
	#define LOG_ERROR(msg) std::cout << "Error! " << msg << "\n"
	#define LOG_WARNING(msg) std::cout << "Warning: " << msg << "\n"
	#define LOG_INFO(msg) std::cout << msg << "\n"
#else
	#define LOG_TITLE(title)
	#define LOG_STREAM(s)
	#define SET_LOG_VERBOSE(boolVal)
	#define LOCAL_LOG(msg)
	#define LOCAL_WARNING(msg)
	#define LOCAL_ERROR(msg)
	#define LOCAL_INFO(msg)
	#define LOCAL_VERBOSE(msg)
	#define LOG(msg)
	#define LOG_WARNING(msg)
	#define LOG_ERROR(msg)
	#define LOG_INFO(msg)
	#define LOG_VERBOSE(msg)
#endif


