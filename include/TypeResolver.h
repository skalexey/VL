#pragma once
#include <string>
#include <functional>
#include "vl_fwd.h"

class TypeResolver
{
public:
	typedef std::function<std::string(const vl::Object& object)> FGetTypeId;
	typedef std::function<const vl::Object& (const std::string& protoId)> FGetProto;
	typedef std::function<bool(const vl::Object& object)> FIsType;
	
	TypeResolver() = default;
	TypeResolver(
		const FGetTypeId& fGetTypeId
		, const FGetProto& fGetProto
		, const FIsType& fIsType
	);
	
	operator bool() const;

	std::string GetTypeId(const vl::Object& object) const;
	const vl::Object& GetProto(const std::string& protoId) const;
	bool IsType(const vl::Object& object) const;

protected:
	FGetTypeId mGetTypeId = nullptr;
	FGetProto mGetProto = nullptr;
	FIsType mIsType = nullptr;
};

