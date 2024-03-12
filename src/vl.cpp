#include <assert.h>
#include <sstream>
#include <unordered_set>
#include "vl.h"
#include "vl_visitor.h"
#include "VLNotifications.h"
#include <utils/log.h>
#ifdef VL_LOG_VERBOSE
	SET_LOCAL_LOG_LEVEL(verbose);
#else
	SET_LOCAL_LOG_LEVEL(debug);
#endif
LOG_PREFIX("[VL]: ")
#ifdef LOG_ON
	#include <utils/string_utils.h>
#endif

namespace vl
{
	vl::Object& NullObject()
	{
		static vl::Object nullObject(nullptr);
		return nullObject;
	}

	vl::VarPtr& EmptyVarPtr()
	{
		static vl::VarPtr emptyVarPtr;
		return emptyVarPtr;
	}

	vl::NullVar& EmptyVar()
	{
		static vl::NullVar emptyVar;
		return emptyVar;
	}
	vl::ListVar& EmptyList()
	{
		static vl::ListVar emptyList;
		return emptyList;
	}
	

	VarPtr MakePtr(bool value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<BoolVar>(value));
	}

	VarPtr MakePtr(float value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<NumberVar>(value));
	}

	VarPtr MakePtr(int value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<NumberVar>((float)value));
	}

	VarPtr MakePtr(const char* value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<StringVar>(value));
	}

	VarPtr MakePtr(const std::string& value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<StringVar>(value));
	}

	VarPtr MakePtr(const void* value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<PointerVar>(value));
	}

	VarPtr MakePtr(const ObjectVar& value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<ObjectVar>(value));
	}

	VarPtr MakePtr(const ListVar& value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<ListVar>(value));
	}

	VarPtr MakePtr(const NullVar& value)
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<NullVar>());
	}

	VarPtr MakePtr(const Var& value)
	{
		if (value.is<vl::Bool>())
			return MakePtr(value.as<vl::Bool>().Val());
		else if (value.is<vl::Number>())
			return MakePtr(value.as<vl::Number>().Val());
		else if (value.is<vl::String>())
			return MakePtr(value.as<vl::String>().Val());
		else if (value.is<vl::Pointer>())
			return MakePtr(value.as<vl::Pointer>().Val());
		else if (value.is<vl::Object>())
			return MakePtr(value.as<vl::Object>());
		else if (value.is<vl::List>())
			return MakePtr(value.as<vl::List>());
		else if (value.IsNull())
			return MakePtr(NullVar());
		else
		{
			return VarPtr(nullptr);
		}
	}

	VarPtr MakePtr()
	{
		return std::dynamic_pointer_cast<Var>(std::make_shared<vl::NullVar>());
	}

	// ======= Begin of AbstractVar definitions =======
	Type AbstractVar::GetType() const
	{
		return Type::Count;
	}
	const void* AbstractVar::Data() const
	{
		// Only objects and lists have shared data
		return nullptr;
	}

	vl::VarPtr AbstractVar::Copy() const
	{
		// Default implementation
		return Ptr();
	}

	const VarPtr& AbstractVar::operator[](const char* s) const
	{
		return VarPtr();
	}

	VarPtr& AbstractVar::operator[](const char* s)
	{
		return EmptyVarPtr();
	}
	// ======= End of AbstractVar definitions =======

	// ======= Begin of ObjectVar definitions =======
	// TODO: support
	//ObjectInsertRet ObjectInsertRet::Null()
	//{
	//	return { "", EmptyVar() };
	//}

	//vl::Var& ObjectInsertRet::data()
	//{
	//	return mData ? *mData : EmptyVar();
	//}

	void PropsDataType::Notify(vl::VarPtr info)
	{
		info.as<vl::Object>().Set("who", "vl");
		return Observable::Notify(info);
	}

	void PropsDataType::Update(Observable* sender, vl::VarPtr info)
	{
		auto& o = info.as<vl::Object>();
		if (o.Get("who").as<vl::String>().Val() != "vl")
			return;
		if (auto subscriptionInfo = GetSubscriptionInfo(sender))
		{
			// Redirect nested object update (proto and others)
			// to all subscribers
			INIT_NOTIFY
			vl::Object propUpdate;
			propUpdate.Set("id", subscriptionInfo->title);
			propUpdate.Set("data", info);
			NOTIFY_ADD("propUpdate", propUpdate);
			SEND_NOTIFY(this)
		}
	}

	ObjectVar::ObjectVar(const ObjectDataType& dataPtr)
		: mData(dataPtr)
	{}

	bool ObjectVar::operator==(const VarInterface& right) const
	{
		if (!right.is<vl::Object>())
			return false;
		return right.as<vl::Object>().mData.get() == mData.get();
	}

	bool ObjectVar::operator==(const VarInterface& right)
	{
		return const_cast<const ObjectVar*>(this)->operator==(right);
	}

	bool ObjectVar::Same(const VarInterface& right) const
	{
		// Deep comparison
		if (!right.is<vl::Object>())
			return false;
		auto& right_object = right.as<vl::Object>();
		if (right_object.mData.get() == mData.get())
			return true;
		if (!mData || !right_object.mData)
			return false;
		if (mData->data.size() != right_object.mData->data.size())
			return false;
		for (auto& [propName, prop] : mData->data)
		{
			auto it = right_object.mData->data.find(propName);
			if (it == right_object.mData->data.end())
				return false;
			if (!(prop.Same(it->second)))
				return false;
		}
		return true;
	}

	ObjectVar::operator bool() const
	{
		return !IsNull();
	}

	Type ObjectVar::GetType() const
	{
		return Type::Object;
	}

	std::size_t ObjectVar::Size() const
	{
		return mData ? mData->data.size() : 0;
	}

	VarPtr& ObjectVar::Set(const std::string& propName)
	{
		return Set(propName, MakePtr(NullVar()));
	}

	VarPtr& ObjectVar::Set(const std::string& propName, const Var& value)
	{
		return Set(propName, MakePtr(value));
	}

	VarPtr& ObjectVar::Set(const std::string& propName, const VarPtr& varPtr)
	{
		if (!mData)
			return EmptyVarPtr();

		VarPtr* ret = nullptr;

		if (mData->HasSubscribers())
		{
			INIT_NOTIFY_BEFORE
			auto it = mData->data.find(propName);
			if (it == mData->data.end())
			{
				// Send "before" notification
				SEND_NOTIFY_BEFORE("add", propName, mData)
				// Set the data
				ret = &(mData->data[propName] = varPtr);
			}
			else
			{
				// Send "before" notification
				SEND_NOTIFY_BEFORE("set", propName, mData);
				// Set the data
				ret = &(it->second = varPtr);
			}
			// Send "after" notification
			NOTIFY_AFTER(mData)
		}
		else
		{
			ret = &(mData->data[propName] = varPtr);
		}

		if (ret->is<vl::Object>())
			ret->as<vl::Object>().Attach(mData.get(), propName);
		//else if (ret->IsList())
		//	ret->as<vl::List>().Attach(mData.get(), propName);
		
		LOCAL_VERBOSE(utils::format_str("Create new variable %p in %p with name '%s'", ret, this, propName.c_str()));

		return *ret;
	}

	const VarPtr& ObjectVar::Get(const std::string& propName) const
	{
		if (!mData)
			return EmptyVarPtr();
		auto it = mData->data.find(propName);
		if (it != mData->data.end())
			return it->second;
		else
		{
			if (auto& proto = GetPrototype())
				return proto.Get(propName);
		}
		return EmptyVarPtr();
	}

	VarPtr& ObjectVar::Get(const std::string& propName)
	{
		return const_cast<VarPtr&>(const_cast<const ObjectVar*>(this)->Get(propName));
	}

	const VarPtr& ObjectVar::operator[](const char* s) const
	{
		return Get(s);
	}

	VarPtr& ObjectVar::operator[](const char* s)
	{
		auto& ptr = Get(s);
		if (!ptr)
			return Set(s, VarPtr());
		return ptr;
	}
	
	bool ObjectVar::Has(const std::string& propName) const
	{
		if (!mData)
			return false;
		auto it = mData->data.find(propName);
		if (it == mData->data.end())
		{
			if (auto& proto = GetPrototype())
				return proto.Has(propName);
			return false;
		}
		return true;
	}

	bool ObjectVar::HasOwn(const std::string& propName) const
	{
		if (!mData)
			return false;
		auto it = mData->data.find(propName);
		return it != mData->data.end();
	}

	std::shared_ptr<std::string> ObjectVar::GetRelativePath(const std::string& propName) const
	{
		return getRelativePathRecursive(propName);
	}

	std::shared_ptr<std::string> ObjectVar::getRelativePathRecursive(const std::string& propName, const std::string& path) const
	{
		if (!mData)
			return nullptr;
		auto it = mData->data.find(propName);
		if (it == mData->data.end())
		{
			if (auto& proto = GetPrototype())
				return proto.getRelativePathRecursive(propName, (path.empty() ? "proto" : (path + ".proto")));
			return nullptr;
		}
		return std::make_shared<std::string>(path);
	}

	bool ObjectVar::Overridden(const std::string& propName) const
	{
		return overriddenRecursive(propName);
	}

	bool ObjectVar::overriddenRecursive(const std::string& propName, int count) const
	{
		if (!mData)
			return false;
		auto it = mData->data.find(propName);
		if (it == mData->data.end())
		{
			// Not found. Go through the proto chain and search there
			if (auto& proto = GetPrototype())
				return proto.overriddenRecursive(propName, count);
			return false;
		}
		// Found one
		if (count == 0)
		{
			if (auto& proto = GetPrototype())
				return proto.overriddenRecursive(propName, ++count);
			else
				return false;
		}
		else
			return true;
	}

	std::size_t ObjectVar::PropCount() const
	{
		if (!mData)
			return 0;
		return mData->data.size();
	}

	bool ObjectVar::RemoveProperty(const std::string& propName)
	{
		if (!mData)
			return false;
		
		auto it = mData->data.find(propName);
		if (it != mData->data.end())
		{
			if (mData->HasSubscribers())
			{
				NOTIFY_BEFORE("remove", propName, mData)
				// Unsubscribe from it's updates
				auto& v = it->second;
				if (v.is<vl::Object>())
					v.as<vl::Object>().Detach(mData.get());
				else if (v.is<vl::List>())
					v.as<vl::List>().Detach(mData.get());

				// Erase the data
				mData->data.erase(it);
				// Send "after" notification
				NOTIFY_AFTER(mData)
			}
			else
			{
				mData->data.erase(it);
			}

			return true;
		}
		return false;
	}

	bool ObjectVar::RenameProperty(const std::string& propName, const std::string& newName)
	{
        if (Has(newName))
            return false;
		if (Has(propName))
			if (propName != newName)
			{
				auto varPtr = mData->data[propName];
				if (mData->HasSubscribers())
				{
					INIT_NOTIFY_BEFORE
					NOTIFY_ADD("newId", newName)
					SEND_NOTIFY_BEFORE("rename", propName, mData);
					
					mData->data.erase(propName);
					mData->data.emplace(newName, varPtr);

					NOTIFY_AFTER(mData)
				}
				else
				{
					mData->data.erase(propName);
					mData->data.emplace(newName, varPtr);
				}
				return true;
			}
		return false;
	}
	
	bool ObjectVar::Accept(Visitor& v, const char* name) const
	{
		if (!v.VisitObject(*this, name))
			return false;
		if (!mData)
			return true;
		// Process 'proto' field first
		// to avoid overwriting overriden properties
		auto it = mData->data.find("proto");
		if (it != mData->data.end())
			it->second->Accept(v, it->first.c_str());
		for (auto& [propName, prop] : mData->data)
		{
			if (propName == "proto")
				continue; // Already processed above
			if (!prop->Accept(v, propName.c_str()))
				continue;
		}
		if (!v.EndVisitObject(*this, name))
			return false;
		return true;
	}

	VarPtr ObjectVar::Copy() const
	{
		ObjectVar copy;
		if (!mData)
		{
			copy.mData = nullptr;
			return MakePtr(copy);
		}
		for (auto& [name, prop] : mData->data)
			if (name == "proto")
				copy.Set(name.c_str(), prop);
			else
				copy.Set(name.c_str(), prop->Copy());
		return MakePtr(copy);
	}

	bool ObjectVar::ForeachProp(const std::function<bool(const std::string&, const vl::Var&)>& pred, bool recursive) const
	{
		if (!pred)
			return false;
		if (!mData)
			return false;
		if (!recursive)
		{
			for (auto& [propName, value] : mData->data)
				if (!pred(propName, *value))
					return false;
		}
		else
		{
			std::unordered_set<std::string> keys;
			vl::VarPtr proto = nullptr;
			for (auto& [propName, value] : mData->data)
			{
				if (!pred(propName, *value))
					return false;
				keys.emplace(propName);
				if (propName == "proto")
					proto = value;
			}
			if (proto != nullptr)
				if (proto->is<vl::Object>())
					if (!const_cast<const vl::ObjectVar&>(proto->as<vl::Object>()).ForeachProp([&](auto& k, auto& v) {
						if (keys.find(k) != keys.end())
							return true;
						keys.emplace(k);
						if (!pred(k, v))
							return false;
						return true;
					}, recursive))
						return false;
		}
		return true;
	}

	bool ObjectVar::ForeachProp(const std::function<bool(const std::string&, vl::Var&)>& pred, bool recursive)
	{
		// TODO: optimize performance
		auto pred2 = [&](const std::string& s, const vl::Var& v) {
			if (!pred(s, const_cast<vl::Var&>(v)))
				return false;
			return true;
		};
		return const_cast<const ObjectVar*>(this)->ForeachProp(pred2, recursive);
	}
	
	void ObjectVar::SetPrototype(const vl::Object& proto)
	{
		Set("proto", proto);
	}

	Object& ObjectVar::GetPrototype() const
	{
		if (mData)
		{
			auto it = mData->data.find("proto");
			if (it != mData->data.end())
				if (it->second->is<vl::Object>())
					return it->second->as<vl::Object>();
		}
		return NullObject();
	}

	void ObjectVar::Clear(bool recursive) {
		if (!mData)
			return;

		auto clear = [&]() {
			if (recursive)
			{
				ForeachProp([&](auto& k, auto& e) {
					if (e.is<vl::Object>())
						const_cast<vl::Var&>(e).as<vl::Object>().Clear(recursive);
					else if (e.is<vl::List>())
						const_cast<vl::Var&>(e).as<vl::List>().Clear(recursive);
					return true;
				});
				mData->data.clear();
			}
			else
				mData->data.clear();
		};

		if (mData->HasSubscribers())
		{
			NOTIFY_BEFORE("clear", true, mData);
			clear();
			NOTIFY_AFTER(mData)
		}
		else
			clear();
	}

	std::string ObjectVar::ToStr() const
	{
		if (auto& proto = GetPrototype())
			return "{someProto}";
		return "{}";
	}


	
	// ======= End of ObjectVar definitions =======

	// ======= Begin of BoolVar definitions =======
	Type BoolVar::GetType() const
	{
		return Type::Bool;
	}

	bool BoolVar::Accept(Visitor& v, const char* name) const
	{
		return v.VisitBool(*this, name);
	}

	std::string BoolVar::ToStr() const
	{
		return Val() ? "true" : "false";
	}

	BoolVar& BoolVar::operator=(bool val)
	{
		mData = val;
		return *this;
	}

	bool BoolVar::operator==(const VarInterface& right) const
	{
		if (!right.is<vl::Bool>())
			return false;
		return right.as<vl::Bool>().mData == mData;
	}

	bool BoolVar::Same(const VarInterface& right) const
	{
		return *this == right;
	}

	// ======= End of BoolVar definitions =======

	// ======= Begin of NumberVar definitions =======
	Type NumberVar::GetType() const
	{
		return Type::Number;
	}

	bool NumberVar::Accept(Visitor& v, const char* name) const
	{
		return v.VisitNumber(*this, name);
	}

	std::string NumberVar::ToStr() const
	{
		std::stringstream ss;
		ss << Val();
		return ss.str();
	}

	bool NumberVar::Same(const VarInterface& right) const
	{
		return *this == right;
	}

	bool NumberVar::operator==(const VarInterface& right) const
	{
		if (!right.is<vl::Number>())
			return false;
		return right.as<vl::Number>().mData == mData;
	}

	NumberVar& NumberVar::operator=(int val)
	{
		mData = float(val);
		return *this;
	}

	NumberVar& NumberVar::operator=(double val)
	{
		mData = float(val);
		return *this;
	}

	NumberVar& NumberVar::operator=(float val)
	{
		mData = val;
		return *this;
	}
	// ======= End of NumberVar definitions =======

	// ======= Begin of StringVar definitions =======
	Type StringVar::GetType() const
	{
		return Type::String;
	}

	bool StringVar::Accept(Visitor& v, const char* name) const
	{
		return v.VisitString(*this, name);
	}

	std::string StringVar::ToStr() const
	{
		return Val();
	}

	bool StringVar::Same(const VarInterface& right) const
	{
		return *this == right;
	}

	bool StringVar::operator==(const VarInterface& right) const
	{
		if (!right.is<vl::String>())
			return false;
		return right.as<vl::String>().Val().compare(mData) == 0;
	}
	
	// ======= End of StringVar definitions =======

	// ======= Begin of PointerVar definitions =======
	Type PointerVar::GetType() const
	{
		return Type::Pointer;
	}

	bool PointerVar::Accept(Visitor& v, const char* name) const
	{
		return v.VisitPointer(*this, name);
	}

	std::string PointerVar::ToStr() const
	{
		std::stringstream ss;
		ss << Val();
		return ss.str();
	}

	bool PointerVar::operator==(const VarInterface& right) const
	{
		if (!right.is<vl::Pointer>())
			return false;
		return right.as<vl::Pointer>().mData == mData;
	}

	bool PointerVar::Same(const VarInterface& right) const
	{
		return *this == right;
	}

	// ======= Begin of ListVar definitions =======
	Type ListVar::GetType() const
	{
		return Type::List;
	}

	void ListDataType::Notify(vl::VarPtr info)
	{
		info->as<vl::Object>().Set("who", "vl");
		return Observable::Notify(info);
	}

	void ListDataType::Update(Observable* sender, vl::VarPtr info)
	{
		auto& o = info->as<vl::Object>();
		if (o.Get("who").as<vl::String>().Val() != "vl")
			return;
		if (auto subscriptionInfo = GetSubscriptionInfo(sender))
		{
			// Redirect nested object update (proto and others)
			// to all subscribers
			INIT_NOTIFY
			vl::Object propUpdate;
			propUpdate.Set("id", subscriptionInfo->title);
			propUpdate.Set("data", info);
			NOTIFY_ADD("propUpdate", propUpdate);
			SEND_NOTIFY(this)
		}
	}

	ListInsertRet ListInsertRet::Null()
	{
		return { -1, EmptyVar() };
	}

	vl::Var& ListInsertRet::data()
	{
		return mData ? *mData : EmptyVar();
	}

	bool ListVar::Accept(Visitor& v, const char* name) const
	{
		if (!v.VisitList(*this, name))
			return false;
		if (!mData)
			return true;
		for (auto& prop : mData->data)
			if (!prop->Accept(v))
				continue;
		if (!v.EndVisitList(*this, name))
			return false;
		return true;
	}

	void ListVar::Clear(bool recursive)
	{
		if (!mData)
			return;

		auto clear = [&]() {
			if (recursive)
			{
				auto sz = Size();
				for (int i = 0; i < sz; i++)
				{
					auto& e = mData->data[i];
					if (e.is<vl::Object>())
						e.as<vl::Object>().Clear(recursive);
					else if (e.is<vl::List>())
						e.as<vl::List>().Clear(recursive);
				}
				mData->data.clear();
			}
			else
				mData->data.clear();
		};

		if (mData->HasSubscribers())
		{
			NOTIFY_BEFORE("clear", true, mData);
			clear();
			NOTIFY_AFTER(mData)
		}
		else
			clear();
	}

	bool ListVar::Remove(int index)
	{
		if (mData)
		{
			if (index >= 0 && index < Size())
			{
				NOTIFY_BEFORE("remove", index, mData)
				mData->data.erase(mData->data.begin() + index);
				NOTIFY_AFTER(mData)
				return true;
			}
		}
		return false;
	}

	const VarPtr& ListVar::At(int index) const
	{
		// Don't check the range. Should be checked on the level above
		if (mData && index >= 0 && index < mData->data.size())
			return mData->data[index];
		return vl::EmptyVarPtr();
	}

	VarPtr& ListVar::At(int index)
	{
		return const_cast<VarPtr&>(const_cast<const ListVar*>(this)->At(index));
	}

	ListInsertRet ListVar::Add(const VarPtr& varPtr, int indexBefore)
	{
		assert(mData);
		if (!mData)
			return ListInsertRet::Null();
		auto sz = mData->data.size();
		
		INIT_NOTIFY_BEFORE
		if (indexBefore >= 0 && indexBefore < sz)
		{
			NOTIFY_ADD("indexBefore", indexBefore)
			SEND_NOTIFY_BEFORE("add", indexBefore, mData)
			auto& result = **mData->data.insert(mData->data.begin() + indexBefore, varPtr);
			NOTIFY_AFTER(mData)
			return { indexBefore, result };
		}
		else
		{
			SEND_NOTIFY_BEFORE("add", int(sz), mData)
			mData->data.push_back(varPtr);
			NOTIFY_AFTER(mData)
			return { int(sz), *mData->data.back() };
		}
	}

	ListInsertRet ListVar::Set(int index)
	{
		return Set(index, MakePtr(NullVar()));
	}

	ListInsertRet ListVar::Set(int index, const Var& value)
	{
		return Set(index, MakePtr(value));
	}

	ListInsertRet ListVar::Set(int index, const VarPtr& varPtr)
	{
		if (!mData)
			return ListInsertRet::Null();
		if (index < 0 || index >= mData->data.size())
			return ListInsertRet::Null();

		NOTIFY_BEFORE("set", index, mData)
		auto& result = *(mData->data[index] = varPtr);
		
		LOCAL_VERBOSE(utils::format_str("Create new variable %p in %p at index '%d'", &result, this, index));
		
		NOTIFY_AFTER(mData)
		return { index, result };
	}

	Var& ListVar::Back()
	{
		return mData ? *mData->data.back() : EmptyVar();
	}

	bool ListVar::IsEmpty() const
	{
		return mData ? mData->data.empty() : true;;
	}

	VarPtr ListVar::Copy() const
	{
		ListVar copy;
		if (!mData)
		{
			copy.mData = nullptr;
			return MakePtr(copy);
		}
		for (auto& prop : mData->data)
			if (prop->is<vl::Object>())
				copy.Add(prop->as<vl::Object>().Copy());
			else if (prop->is<vl::List>())
				copy.Add(prop->as<vl::List>().Copy());
			else
				copy.Add(prop);
		return MakePtr(copy);
	}

	std::string ListVar::ToStr() const
	{
		return "[]";
	}

	bool ListVar::Same(const VarInterface& right) const
	{
		if (!right.is<vl::List>())
			return false;
		auto& right_list = right.as<vl::List>();
		if (right_list.mData.get() == mData.get())
			return true;
		if (!mData || !right_list.mData)
			return false;
		if (mData->data.size() != right_list.mData->data.size())
			return false;
		for (int i = 0; i < mData->data.size(); i++)
			if (!(mData->data[i].Same(right_list.mData->data[i])))
				return false;
		return true;
	}

	bool ListVar::operator==(const VarInterface& right) const
	{
		return mData == right.as<vl::List>().mData;
	}

	// ======= End of ListVarDefinitions =======
	bool NullVar::Accept(Visitor& v, const char* name) const
	{
		return v.VisitNull(*this, name);
	}

	Type NullVar::GetType() const
	{
		return Type::Null;
	}

	bool NullVar::operator==(const VarInterface& right) const
	{
		return right.IsNull();
	}

	bool NullVar::Same(const VarInterface& right) const
	{
		return *this == right;
	}
}
