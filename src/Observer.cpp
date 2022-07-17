#include <assert.h>
#include "Observer.h"
#include "Observable.h"
#include <utils/Log.h>
#ifdef LOG_ON
	#include <utils/string_utils.h>
#endif

namespace vl
{
	// The maximum lifetime provided
	std::unordered_map<Observer*, std::unordered_map<Observable*, SubscriptionInfo>>&
		Observer::mSubscriptions
			= *(new std::unordered_map<Observer*, std::unordered_map<Observable*, SubscriptionInfo>>());

	Observer::~Observer()
	{
		LOG_VERBOSE(utils::format_str("~Observer() %p", this));

		auto it = mSubscriptions.find(this);
		if (it != mSubscriptions.end())
		{
			LOG_VERBOSE(utils::format_str("	Found %d subscriptions", mSubscriptions.size()));

			auto& sc = it->second;
			for (auto& [observable, info] : sc)
			{
				LOG_VERBOSE(utils::format_str("	Unsubscribe from %p", observable));
				auto observers = observable->GetObservers();
				assert(observers != nullptr);
				auto it = std::find(observers->begin(), observers->end(), this);
				assert(it != observers->end());
				observers->erase(it);
			}
			mSubscriptions.erase(it);
		}
	}

	const std::unordered_map<Observable*, SubscriptionInfo>* Observer::GetSubscriptions() const
	{
		auto it = mSubscriptions.find((Observer*)this);
		if (it != mSubscriptions.end())
			return &it->second;
		return nullptr;
	}

	std::unordered_map<Observable*, SubscriptionInfo>* Observer::GetSubscriptions()
	{
		return const_cast<std::unordered_map<Observable*, SubscriptionInfo>*>
			(const_cast<const Observer*>(this)->GetSubscriptions());
	}

	const SubscriptionInfo* Observer::GetSubscriptionInfo(const Observable* o) const
	{
		if (auto subs = GetSubscriptions())
		{
			auto it = subs->find((Observable*)o);
			if (it != subs->end())
				return &it->second;
		}
		return nullptr;
	}

	SubscriptionInfo* Observer::GetSubscriptionInfo(Observable* o)
	{
		return const_cast<SubscriptionInfo*>
			(const_cast<const Observer*>(this)->GetSubscriptionInfo(o));
	}
}