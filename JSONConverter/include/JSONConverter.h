#pragma once

#include "vl.h"
#include "JSONDefs.h"

namespace vl
{
	class JSONConverter
	{
	public:
		bool Store(Object& object, const vl::Object& context, const char* filePath, const CnvParams& params = CnvParams());
		bool JSONStr(Object& object, const vl::Object& context, std::string& out, const CnvParams& params = CnvParams());
		std::string JSONStr(Object& object, const vl::Object& context, const CnvParams& params = CnvParams());
		bool Load(Object& object, const char* filePath);
	};
}
