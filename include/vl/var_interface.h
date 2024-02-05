#pragma once
#include <vl_fwd.h>

namespace vl
{
	class VarInterface
	{
	public:
		virtual const BoolVar& AsBool() const = 0;
		virtual const NumberVar& AsNumber() const = 0;
		virtual const StringVar& AsString() const = 0;
		virtual const ObjectVar& AsObject() const = 0;
		virtual const ListVar& AsList() const = 0;
		virtual BoolVar& AsBool() = 0;
		virtual NumberVar& AsNumber() = 0;
		virtual StringVar& AsString() = 0;
		virtual ObjectVar& AsObject() = 0;
		virtual ListVar& AsList() = 0;
		virtual bool IsBool() const = 0;
		virtual bool IsNumber() const = 0;
		virtual bool IsString() const = 0;
		virtual bool IsObject() const = 0;
		virtual bool IsList() const = 0;
		virtual bool IsNull() const = 0;
		virtual Type GetType() const = 0;
		virtual operator bool() const { return !IsNull(); }
		virtual std::string ToStr() const = 0;
		virtual const void* Data() const = 0;
		virtual vl::VarPtr Copy() const = 0;
		virtual const VarPtr& operator[](const char* s) const = 0;
		virtual VarPtr& operator[](const char* s) = 0;
	};
}
