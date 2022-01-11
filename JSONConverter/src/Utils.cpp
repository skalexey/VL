//
//  Utils.cpp
//  Cloud Raiders Debug
//
//  Created by skorokhodov on 11/22/17.
//

#include <string>
#include <chrono>
#include <stdio.h>
#include <time.h>
#include <cstdint>
#include <cstdarg>
#include "Utils.h"

using namespace Utils;

void Utils::Log(const char* fmt, va_list args)
{
    char buf[512];
    vsnprintf(buf, 512, fmt, args);
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    time_t tnow = std::chrono::system_clock::to_time_t(now);
    tm *date = std::localtime(&tnow);
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    printf("%02i:%02i:%02i:%03li %s\n", date->tm_hour, date->tm_min, date->tm_sec, ms % 1000, buf);
}

void Utils::Log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Log(fmt, args);
    va_end(args);
}

void Utils::Warn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Log("Warning: %s", fmt, args);
    va_end(args);
}

unsigned Utils::Hash(const std::string& data)
{
	uint32_t hash = 0;
	const char* str = data.c_str();
	while (unsigned char c = (unsigned char)*str)
	{
		hash = c + (hash << 6) + (hash << 16) - hash;
		str++;
	}
	return hash;
}
