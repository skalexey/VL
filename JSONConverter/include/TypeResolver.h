#pragma once
#include <string>
#include <functional>
#include "vl_fwd.h"

class TypeResolver
{
public:
	typedef std::function<std::string(const vl::Object& object)> FGetTypeId;
	typedef std::function<vl::Object& (const std::string& protoId)> FGetProto;
	
	TypeResolver() = default;
	TypeResolver(const FGetTypeId& fGetTypeId, const FGetProto& fGetProto);
	
	operator bool() const;

	std::string GetTypeId(const vl::Object& object) const;
	const vl::Object& GetProto(const std::string& protoId) const;

protected:
	FGetTypeId mGetTypeId = nullptr;
	FGetProto mGetProto = nullptr;
};

