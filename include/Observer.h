#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "vl_fwd.h"

namespace vl
{
	class Observable;

	struct SubscriptionInfo
	{
		Observable* observervable;
		std::string title;
	};

	class Observer
	{
		friend class Observable;
	protected:
		Observer() = default;

	public:
		virtual ~Observer();
		virtual void Update(Observable* sender, vl::VarPtr info = nullptr) = 0;
		const SubscriptionInfo* GetSubscriptionInfo(const Observable* o) const;
		const std::unordered_map<Observable*, SubscriptionInfo>* GetSubscriptions() const;

	protected:
		std::unordered_map<Observable*, SubscriptionInfo>* GetSubscriptions();
		SubscriptionInfo* GetSubscriptionInfo(Observable* o);

	private:
		static std::unordered_map<Observer*, std::unordered_map<Observable*, SubscriptionInfo>>& mSubscriptions;
		std::string mTitle;
	};
}