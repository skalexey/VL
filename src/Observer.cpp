#include <assert.h>
#include "Observer.h"
#include "Observable.h"
#include "Log.h"
#ifdef LOG_ON
	#include "Utils.h"
#endif

namespace vl
{
	std::unordered_map<Observer*, std::vector<Observable*>> Observer::mSubscriptions;

	Observer::~Observer()
	{
		LOG_VERBOSE(Utils::FormatStr("~Observer() %p", this));

		auto it = mSubscriptions.find(this);
		if (it != mSubscriptions.end())
		{
			LOG_VERBOSE(Utils::FormatStr("	Found %d subscriptions", mSubscriptions.size()));

			auto& sc = it->second;
			for (auto observable : sc)
			{
				LOG_VERBOSE(Utils::FormatStr("	Unsubscribe from %p", observable));
				auto observers = observable->GetObservers();
				assert(observers != nullptr);
				auto it = std::find(observers->begin(), observers->end(), this);
				assert(it != observers->end());
				observers->erase(it);
			}
			mSubscriptions.erase(it);
		}
	}

	std::vector<Observable*>* Observer::GetSubscriptions()
	{
		auto it = mSubscriptions.find(this);
		if (it != mSubscriptions.end())
			return &it->second;
		return nullptr;
	}
}