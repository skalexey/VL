#pragma once

#include <vl.h>

namespace vl
{
	class VLDataInterface {
	public:
		virtual void SetData(const vl::VarPtr& data) = 0;
		virtual void SetData(const vl::Var& data) {
			SetData(vl::MakePtr(data));
		};
		virtual bool HasData() const = 0;
	};
}
