#include <algorithm>
#include <assert.h>
#include "Observable.h"
#include "Observer.h"
#include "Log.h"
#ifdef LOG_ON
	#include "Utils.h"
#endif

namespace vl
{
	Observable::ObserversStorage* Observable::mObserversStorage = new Observable::ObserversStorage();

	void Observable::Unsubscribe(Observer* o)
	{
		LOG_VERBOSE(Utils::FormatStr("	%p->Unsubscribe(%p)", this, o));
		auto sc = o->GetSubscriptions();
		assert(sc != nullptr);
		auto scIt = std::find(sc->begin(), sc->end(), this);
		assert(scIt != sc->end());
		sc->erase(scIt);
	}

	Observable::Observable()
	{
		LOG_VERBOSE(Utils::FormatStr("Observable() %p", this));
	}

	bool Observable::Attach(Observer* o)
	{
		LOG_VERBOSE(Utils::FormatStr("%p->Attach(%p)", this, o));
		if (auto observers = GetObservers())
		{
			auto it = std::find(observers->begin(), observers->end(), o);
			if (it != observers->end())
			{
				LOG_VERBOSE("	Already attached");
				return false;
			}
		}
		mObserversStorage->GetObservers()[this].push_back(o);
		auto& sc = Observer::mSubscriptions[o];
		assert(std::find(sc.begin(), sc.end(), this) == sc.end());
		sc.push_back(this);
		return true;
	}

	bool Observable::Detach(Observer* o)
	{
		LOG_VERBOSE(Utils::FormatStr("%p->Detach(%p)", this, o));
		if (auto observers = GetObservers())
		{
			auto it = std::find(observers->begin(), observers->end(), o);
			if (it != observers->end())
			{
				observers->erase(it);
				Unsubscribe(o);
				return true;
			}
		}
		LOG_WARNING(Utils::FormatStr("An Attempt to detach not registered observer %p from subject %p", o, this));
		return false;
	}

	Observable::~Observable()
	{
		LOG_VERBOSE(Utils::FormatStr("~Observable() %p", this));
		if (auto observers = GetObservers())
		{
			LOG_VERBOSE(Utils::FormatStr("	Found %d observers", observers->size()));
			for (auto o : *observers)
				Unsubscribe(o);
			mObserversStorage->GetObservers().erase(this);
		}
	}

	void Observable::Notify(vl::VarPtr info)
	{
		auto& observers = mObserversStorage->GetObservers();
		auto it = observers.find(this);
		if (it == observers.end())
			return;
		for (auto o : it->second)
			o->Update(info);
	}

	std::vector<Observer*>* Observable::GetObservers()
	{
		auto& observers = mObserversStorage->GetObservers();
		auto it = observers.find(this);
		if (it != observers.end())
			return &it->second;
		return nullptr;
	}
}
