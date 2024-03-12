#pragma once
#include <vl_fwd.h>
#include <utils/entity.h>
namespace vl
{
	class VarInterface : public utils::entity
	{
	public:
		virtual Type GetType() const = 0;
		virtual bool IsNull() const {
			return false;
		}
		virtual operator bool() const {
			return !IsNull();
		}
		virtual std::string ToStr() const = 0;
		virtual const void* Data() const = 0;
		virtual vl::VarPtr Copy() const = 0;
		virtual const VarPtr& operator[](const char* s) const = 0;
		virtual VarPtr& operator[](const char* s) = 0;
		virtual bool Same(const VarInterface& right) const = 0;
	};
}
