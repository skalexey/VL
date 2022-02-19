#pragma once
#include <vector>
#include <unordered_map>
#include "Observer.h"
#include "vl_fwd.h"

namespace vl
{
	class Observer;

	class Observable
	{
		friend class Observer;

	public:
		Observable();
		bool Attach(Observer* o);
		bool Detach(Observer* o);
		virtual ~Observable();
		void Notify(vl::VarPtr info = nullptr);

	protected:
		std::vector<Observer*>* GetObservers();
		void Unsubscribe(Observer* o);

	private:
		static std::unordered_map<Observable*, std::vector<Observer*>> mObservers;
	};
}