#pragma once

#include <string>
#include "vl.h"
#include "JSONDefs.h"
#include "TypeResolver.h"

namespace vl
{
	class JSONConverter
	{
	public:
		bool Store(Object& object, const TypeResolver& typeResolver, const std::string& filePath, const CnvParams& params = CnvParams());
		bool JSONStr(const Object& object, const TypeResolver& typeResolver, std::string& out, const CnvParams& params = CnvParams());
		std::string JSONStr(const Object& object, const TypeResolver& typeResolver, const CnvParams& params = CnvParams());
		bool Load(Object& object, const std::string& filePath, const TypeResolver& typeResolver = TypeResolver());
	};
}
