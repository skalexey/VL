#pragma once
#include <unordered_map>
#include <vector>
#include "vl_fwd.h"

namespace vl
{
	class Observable;

	class Observer
	{
		friend class Observable;
	public:
		virtual ~Observer();
		virtual void Update(vl::VarPtr info = nullptr) = 0;

	protected:
		std::vector<Observable*>* GetSubscriptions();

	private:
		static std::unordered_map<Observer*, std::vector<Observable*>> mSubscriptions;
	};
}