#pragma once

template <typename TCollection, typename TValue>
struct MapReturnValue {
	MapReturnValue(typename TCollection::const_iterator it, bool valid)
		: mIt(it)
		, mValid(valid)
	{}

	inline const TValue& val() {
		static const TValue emptyValue;
		if (mValid)
			return mIt->second;
		else
			return emptyValue;
	}

	inline operator bool() { return mValid; }

	inline static MapReturnValue<TCollection, TValue> Null() {
		return { typename TCollection::const_iterator(), false }
	}
private:
	bool mValid = false;
	typename TCollection::const_iterator mIt;
};