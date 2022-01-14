#include "vl.h"
#include "vl.h"
#include "vl.h"
#include "vl.h"
#include "vl.h"
#include <assert.h>
#include "vl.h"
#include "visitor.h"

namespace vl
{
	NullVar emptyVar;

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

	VarPtr MakePtr(Var& value)
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
	BoolVar& AbstractVar::AsBool()
	{
		// Default implementation
		static BoolVar emptyVar;
		return emptyVar;
	}

	NumberVar& AbstractVar::AsNumber()
	{
		// Default implementation
		static NumberVar emptyVar;
		return emptyVar;
	}

	StringVar& AbstractVar::AsString()
	{
		// Default implementation
		static StringVar emptyVar;
		return emptyVar;
	}

	ObjectVar& AbstractVar::AsObject()
	{
		// Default implementation
		static ObjectVar emptyVar;
		return emptyVar;
	}

	ListVar& AbstractVar::AsList()
	{
		// Default implementation
		static ListVar emptyVar;
		return emptyVar;
	}
	// ======= End of AbstractVar definitions =======

	// ======= Begin of ObjectVar definitions =======

	//Var& ObjectVar::Set(const char* propName, const Var& value)
	//{
	//	assert(mData);
	//	return *((*mData)[propName] = value.Ptr());
	//}

	bool ObjectVar::operator==(const ObjectVar& right)
	{
		return right.mData.get() == mData.get();
	}

	Var& ObjectVar::Set(const std::string& propName)
	{
		return Set(propName, MakePtr(NullVar()));
	}

	Var& ObjectVar::Set(const std::string& propName, Var& value)
	{
		return Set(propName, MakePtr(value));
	}

	Var& ObjectVar::Set(const std::string& propName, const VarPtr& varPtr)
	{
		if (!mData)
			return emptyVar;
		return *((*mData)[propName] = varPtr);
	}

	Var& ObjectVar::Get(const std::string& propName)
	{
		if (!mData)
			return emptyVar;
		auto it = mData->find(propName);
		if (it != mData->end())
			return *it->second;
		else
		{
			it = mData->find("proto");
			if (it != mData->end())
				if (it->second->IsObject())
					return it->second->AsObject().Get(propName);
		}
		return emptyVar;
	}
	
	bool ObjectVar::Has(const std::string& propName)
	{
		if (!mData)
			return false;
		auto it = mData->find(propName);
		return it != mData->end();
	}

	bool ObjectVar::RemoveProperty(const std::string& propName)
	{
		if (!mData)
			return false;
		mData->erase(propName);
		return true;
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

	bool ObjectVar::ForeachProp(const std::function<bool(const std::string&, vl::Var&)>& pred)
	{
		if (!pred)
			return false;
		if (!mData)
			return false;
		for (auto& [propName, value] : *mData)
			if (!pred(propName, *value))
				return false;
		return true;
	}
	
	void ObjectVar::SetPrototype(const vl::Object& proto)
	{
		Set("proto", proto);
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

	bool NumberVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitNumber(*this, name);
	}

	bool StringVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitString(*this, name);
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

	Var& ListVar::Add(const VarPtr& varPtr)
	{
		if (!mData)
			return emptyVar;
		mData->push_back(varPtr);
		return *mData->back();
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
	// ======= End of ListVarDefinitions =======
	bool NullVar::Accept(Visitor& v, const char* name)
	{
		return v.VisitNull(*this, name);
	}

}
