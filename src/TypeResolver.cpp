#include "TypeResolver.h"
#include "vl.h"
#include <utils/Log.h>
#include <utils/string_utils.h>

TypeResolver::TypeResolver(
	const FGetTypeId& fGetTypeId
	, const FGetProto& fGetProto
	, const FIsType& fIsType
)
	: mGetTypeId(fGetTypeId)
	, mGetProto(fGetProto)
	, mIsType(fIsType)
{}

TypeResolver::operator bool() const
{
	return mGetTypeId || mGetProto;
}

std::string TypeResolver::GetTypeId(const vl::Object& object) const
{
	if (mGetTypeId)
		return mGetTypeId(object);
	else
		LOG_ERROR(utils::format_str("Trying to resolve type id on object %p using uninitialized TypeResolver", &object));
	return "";
}

const vl::Object& TypeResolver::GetProto(const std::string& protoId) const
{
	if (mGetProto)
		return mGetProto(protoId);
	else
		LOG_ERROR(utils::format_str("Trying to resolve prototype '%s' using uninitialized TypeResolver", protoId.c_str()));
	return vl::nullObject;
}

bool TypeResolver::IsType(const vl::Object& object) const
{
	if (mIsType)
		return mIsType(object);
	else
		LOG_ERROR(utils::format_str("Trying to know wheter an object %p is a type using uninitialized TypeResolver", &object));
	return false;
}
