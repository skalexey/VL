#include <assert.h>
#include <sstream>
#include "vl.h"
#include "visitor.h"

namespace vl
{
	vl::NullVar emptyVar;
	vl::Object nullObject(nullptr);
	vl::ListVar emptyList;

	std::string GetTypeId(const vl::Object& value, const vl::Object& context)
	{
		// TODO: avoid iteration
		std::string type;
		auto search = [&](const vl::Var& r) {
			if (r.IsNull())
				return false;
			return !r.AsObject().ForeachProp(
				[&](const std::string& propName, const vl::Var& propVal) {
					if (propVal.AsObject() == value)
					{
						type = propName;
						return false;
					}
					return true;
				});
		};
		if (search(context.Get("types")))
			return type;
		else if (search(context.Get("private")))
			return type;
		return type;
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
		return std::make_shared<NullVar>();
	}

	VarPtr MakePtr(const Var& value)
	{
		if (value.IsBool())
			return MakePtr(value.AsBool().Val());
		else if (value.IsNumber())
			return MakePtr(value.AsNumber().Val());
		else if (value.IsString())
			return MakePtr(value.AsString().Val());
		else if (value.IsObject())
			return MakePtr(value.AsObject());
		else if (value.IsList())
			return MakePtr(value.AsList());
		else if (value.IsNull())
			return MakePtr(NullVar());
		else
			return VarPtr(nullptr);
	}

	VarPtr MakePtr()
	{
		return std::make_shared<vl::NullVar>();
	}

	// ======= Begin of AbstractVar definitions =======
	const BoolVar& AbstractVar::AsBool() const
	{
		// Default implementation
		static BoolVar emptyVar;
		return emptyVar;
	}

	const NumberVar& AbstractVar::AsNumber() const
	{
		// Default implementation
		static NumberVar emptyVar;
		return emptyVar;
	}

	const StringVar& AbstractVar::AsString() const
	{
		// Default implementation
		static StringVar emptyVar;
		return emptyVar;
	}

	const ObjectVar& AbstractVar::AsObject() const
	{
		// Default implementation
		static ObjectVar emptyVar;
		return emptyVar;
	}

	const ListVar& AbstractVar::AsList() const
	{
		// Default implementation
		return emptyList;
	}

	BoolVar& AbstractVar::AsBool()
	{
		return const_cast<BoolVar&>(const_cast<const AbstractVar*>(this)->AsBool());
	}

	NumberVar& AbstractVar::AsNumber()
	{
		return const_cast<NumberVar&>(const_cast<const AbstractVar*>(this)->AsNumber());
	}

	StringVar& AbstractVar::AsString()
	{
		return const_cast<StringVar&>(const_cast<const AbstractVar*>(this)->AsString());
	}

	ObjectVar& AbstractVar::AsObject()
	{
		return const_cast<ObjectVar&>(const_cast<const AbstractVar*>(this)->AsObject());
	}

	ListVar& AbstractVar::AsList()
	{
		// Default implementation
		return const_cast<ListVar&>(const_cast<const AbstractVar*>(this)->AsList());
	}
	// ======= End of AbstractVar definitions =======

	// ======= Begin of ObjectVar definitions =======

	//Var& ObjectVar::Set(const char* propName, const Var& value)
	//{
	//	assert(mData);
	//	return *((*mData)[propName] = value.Ptr());
	//}

	ObjectVar::ObjectVar(const ObjectDataType& dataPtr)
		: mData(dataPtr)
	{}

	bool ObjectVar::operator==(const ObjectVar& right) const
	{
		return right.mData.get() == mData.get();
	}

	bool ObjectVar::operator==(const ObjectVar& right)
	{
		return const_cast<const ObjectVar*>(this)->operator==(right);
	}

	ObjectVar::operator bool() const
	{
		return !IsNull();
	}

	const ObjectVar& ObjectVar::AsObject() const
	{
		return *this;
	}

	ObjectVar& ObjectVar::AsObject()
	{
		return *this;
	}

	int ObjectVar::Size() const
	{
		return mData ? mData->size() : 0;
	}

	Var& ObjectVar::Set(const std::string& propName)
	{
		return Set(propName, MakePtr(NullVar()));
	}

	Var& ObjectVar::Set(const std::string& propName, const Var& value)
	{
		return Set(propName, MakePtr(value));
	}

	Var& ObjectVar::Set(const std::string& propName, const VarPtr& varPtr)
	{
		if (!mData)
			return emptyVar;
		return *((*mData)[propName] = varPtr);
	}

	const Var& ObjectVar::Get(const std::string& propName) const
	{
		if (!mData)
			return emptyVar;
		auto it = mData->find(propName);
		if (it != mData->end())
			return *it->second;
		else
		{
			if (auto& proto = GetPrototype())
				return proto.Get(propName);
		}
		return emptyVar;
	}

	Var& ObjectVar::Get(const std::string& propName)
	{
		return const_cast<Var&>(const_cast<const ObjectVar*>(this)->Get(propName));
	}
	
	bool ObjectVar::Has(const std::string& propName) const
	{
		if (!mData)
			return false;
		auto it = mData->find(propName);
		return it != mData->end();
	}

	int ObjectVar::PropCount() const
	{
		if (!mData)
			return 0;
		return mData->size();
	}

	bool ObjectVar::RemoveProperty(const std::string& propName)
	{
		if (!mData)
			return false;
		return mData->erase(propName) > 0;
	}

	bool ObjectVar::RenameProperty(const std::string& propName, const std::string& newName)
	{
        if (Has(newName))
            return false;
		if (Has(propName))
			if (propName != newName)
			{
				auto varPtr = (*mData)[propName];
				mData->erase(propName);
                mData->emplace(newName, varPtr);
				return true;
			}
		return false;
	}
	
	bool ObjectVar::Accept(Visitor& v, const char* name)
	{
		if (!v.VisitObject(*this, name))
			return false;
		if (!mData)
			return true;
		// Process 'proto' field first
		// to avoid overwriting overriden properties
		auto it = mData->find("proto");
		if (it != mData->end())
			it->second->Accept(v, it->first.c_str());
		for (auto& [propName, prop] : *mData)
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

	ObjectVar ObjectVar::Copy() const
	{
		ObjectVar copy;
		if (!mData)
		{
			copy.mData = nullptr;
			return copy;
		}
		for (auto& [name, prop] : *mData)
			if (prop->IsObject())
				copy.Set(name.c_str(), MakePtr(prop->AsObject().Copy()));
			else if (prop->IsList())
				copy.Set(name.c_str(), MakePtr(prop->AsList().Copy()));
			else
				copy.Set(name.c_str(), prop);
		return copy;
	}

	bool ObjectVar::ForeachProp(const std::function<bool(const std::string&, const vl::Var&)>& pred, bool recursive) const
	{
		if (!pred)
			return false;
		if (!mData)
			return false;
		for (auto& [propName, value] : *mData)
		{
			if (!pred(propName, *value))
				return false;
			if (recursive)
				if (propName == "proto")
					if (value->IsObject())
						const_cast<const vl::ObjectVar&>(value->AsObject()).ForeachProp(pred, recursive);
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
			auto it = mData->find("proto");
			if (it != mData->end())
				if (it->second->IsObject())
					return it->second->AsObject();
		}
		return nullObject;
	}

	std::string ObjectVar::ToStr() const
	{
		if (auto& proto = GetPrototype())
			return "{someProto}";
		return "{}";
	}
	
	// ======= End of ObjectVar definitions =======

	// ======= Begin of BoolVar definitions =======

	// ======= End of BoolVar definitions =======

	// ======= Begin of NumberVar definitions =======

	// ======= End of BoolVar definitions =======

	bool BoolVar::Accept(Visitor& v, const char* name)
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

	bool NumberVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitNumber(*this, name);
	}

	std::string NumberVar::ToStr() const
	{
		std::stringstream ss;
		ss << Val();
		return ss.str();
	}

	NumberVar& NumberVar::operator=(int val)
	{
		mData = val;
		return *this;
	}

	NumberVar& NumberVar::operator=(double val)
	{
		mData = int(val);
		return *this;
	}

	NumberVar& NumberVar::operator=(float val)
	{
		mData = int(val);
		return *this;
	}

	bool StringVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitString(*this, name);
	}

	std::string StringVar::ToStr() const
	{
		return Val();
	}

	// ======= Begin of ListVar definitions =======
	bool ListVar::Accept(Visitor& v, const char* name)
	{
		if (!v.VisitList(*this, name))
			return false;
		if (!mData)
			return true;
		for (auto& prop : *mData)
			if (!prop->Accept(v))
				continue;
		if (!v.EndVisitList(*this, name))
			return false;
		return true;
	}

	int ListVar::Size() const
	{
		return mData ? mData->size() : 0;
	}

	void ListVar::Clear()
	{
		if (mData)
			mData->clear();
	}

	bool ListVar::Remove(int index)
	{
		if (mData)
		{
			if (index >= 0 && index < Size())
				mData->erase(mData->begin() + index);
			return true;
		}
		return false;
	}

	const Var& ListVar::At(int index) const
	{
		// Don't check the range. Should be checked on the level above
		return mData ? *(*mData)[index] : vl::emptyVar;
	}

	Var& ListVar::At(int index)
	{
		return const_cast<Var&>(const_cast<const ListVar*>(this)->At(index));
	}

	Var& ListVar::Add(const VarPtr& varPtr, int indexBefore)
	{
		assert(mData);
		if (!mData)
			return emptyVar;
		if (indexBefore < mData->size())
			return **mData->insert(mData->begin() + indexBefore, varPtr);
		else
		{
			mData->push_back(varPtr);
			return *mData->back();
		}
	}

	Var& ListVar::Set(int index)
	{
		return Set(index, MakePtr(NullVar()));
	}

	Var& ListVar::Set(int index, const Var& value)
	{
		return Set(index, MakePtr(value));
	}

	Var& ListVar::Set(int index, const VarPtr& varPtr)
	{
		if (!mData)
			return emptyVar;
		if (index < 0 || index >= mData->size())
			return emptyVar;
		return *((*mData)[index] = varPtr);
	}

	Var& ListVar::Back()
	{
		return mData ? *mData->back() : emptyVar;
	}

	bool ListVar::IsEmpty() const
	{
		return mData ? mData->empty() : true;;
	}

	ListVar ListVar::Copy() const
	{
		ListVar copy;
		if (!mData)
		{
			copy.mData = nullptr;
			return copy;
		}
		for (auto& prop : *mData)
			if (prop->IsObject())
				copy.Add(MakePtr(prop->AsObject().Copy()));
			else if (prop->IsList())
				copy.Add(MakePtr(prop->AsList().Copy()));
			else
				copy.Add(prop);
		return copy;
	}
	std::string ListVar::ToStr() const
	{
		return "[]";
	}
	// ======= End of ListVarDefinitions =======
	bool NullVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitNull(*this, name);
	}

}
