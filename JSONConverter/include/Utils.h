//
//  Utils.h
//  Cloud Raiders Debug
//
//  Created by skorokhodov on 11/22/17.
//

#ifndef Utils_h
#define Utils_h

namespace Utils
{
	void Log(const char* fmt, ...);
	void Warn(const char* fmt, ...);
    void Log(const char* fmt, va_list args);
	unsigned Hash(const std::string& data);
}

#endif /* Utils_h */
