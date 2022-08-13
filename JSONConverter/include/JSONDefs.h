#pragma once

//#define JSON_LOG_ON
#ifdef JSON_LOG_ON
#include <iostream>
#define JSON_LOG_VERBOSE(msg) std::cout << "[JSON V] " << msg << "\n"
#else
#define JSON_LOG_VERBOSE(msg)
#endif

#include <functional>

// CnvParams
// Conversion parameters
namespace vl
{
	struct CnvParams
	{
		bool pretty = false;
		bool useProtoRefs = true;
		bool storeTypeId = true;
		
		bool operator != (const CnvParams& left) const {
			return !operator==(left);
		}

		bool operator == (const CnvParams& left) const {
			return 	pretty == left.pretty &&
					useProtoRefs == left.useProtoRefs &&
					storeTypeId == left.storeTypeId;
		}
	};
}
