#pragma once

#include <string>
#include "vl.h"
#include "JSONDefs.h"

namespace vl
{
	class JSONConverter
	{
	public:
		bool Store(Object& object, const vl::Object& context, const std::string& filePath, const CnvParams& params = CnvParams());
		bool JSONStr(Object& object, const vl::Object& context, std::string& out, const CnvParams& params = CnvParams());
		std::string JSONStr(Object& object, const vl::Object& context, const CnvParams& params = CnvParams());
		bool Load(Object& object, const std::string& filePath);
	};
}
