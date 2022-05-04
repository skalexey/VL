#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Observer.h"
#include "vl_fwd.h"

namespace vl
{
	class Observer;

	class Observable
	{
		friend class Observer;

	protected:
		Observable();

	public:
		bool Attach(Observer* o, const std::string& title = "");
		bool Detach(Observer* o);
		virtual ~Observable();
		virtual void Notify(vl::VarPtr info = nullptr);
		inline bool HasSubscribers() {
			if (auto subs = GetObservers())
				return !subs->empty(); // TODO get rid of this check
			return false;
		}
		const std::vector<Observer*>* GetObservers() const;

	protected:
		std::vector<Observer*>* GetObservers();
		void Unsubscribe(Observer* o);

	private:
		class ObserversStorage
		{
		public:
			//~ObserversStorage();

			inline std::unordered_map<Observable*, std::vector<Observer*>>& GetObservers() {
				return mObservers;
			}
		private:
			std::unordered_map<Observable*, std::vector<Observer*>> mObservers;
		};

		static ObserversStorage* mObserversStorage;
	};
}
