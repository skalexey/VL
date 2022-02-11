#include <cstring>
#include "JSONLoader.h"
#include "vl.h"
#include "Log.h"

vl::JSONLoader::ContainerInfo* vl::JSONLoader::GetCurrentContainer()
{
	if (mStack.empty())
		return nullptr;
	else
		return &mStack.back();
}

vl::JSONLoader::ContainerInfo* vl::JSONLoader::PushContainer(vl::Var* ptr, const std::string& name)
{
	return &mStack.emplace_back(*ptr, name);
}

void vl::JSONLoader::PopContainer()
{
	if (mStack.empty())
		return;
	mStack.pop_back();
}

bool vl::JSONLoader::AddVar(const vl::VarPtr& ptr)
{
	if (mCurrentProto)
	{
		auto c = GetCurrentContainer();
		LOG_WARNING("Other type than String encountered in the 'proto' field of the container '" << (c ? c->name.c_str() : "<nullptr>") << "'. The prorotype will be ignored");
		mCurrentKey.clear();
		mKeyProcessed = true;
		mCurrentProto = false;
		mUnresolvedRefs.pop_back();
		return true;
	}
	bool isRoot = false;
	if (GetCurrentContainer() == nullptr)
	{
		PushContainer(&mObject, "_root_");
		isRoot = true;
	}
	auto& c = GetCurrentContainer()->var;
	if (c.IsObject())
	{
		if (mCurrentKey.empty() && mKeyProcessed)
		{
			LOG_WARNING("[JSONLoader] Seems like this is a non existent document");
			return false;
		}
		c.AsObject().Set(mCurrentKey.c_str(), ptr);
		mCurrentKey.clear();
		mKeyProcessed = true;
	}
	else if (c.IsList())
	{
		c.AsList().Add(ptr);
	}
	else
	{
		assert(c.IsList() || c.IsObject());
	}
	return true;
}

void vl::JSONLoader::PushNewList(ContainerInfo* parentContainer, const std::string& listName)
{
	PushNewContainer(false, true, parentContainer, listName);
}

void vl::JSONLoader::PushNewObject(ContainerInfo* parentContainer, const std::string& listName)
{
	if (parentContainer->var.IsObject())
		if (parentContainer->name == "types")
		{
			auto newContainer = PushNewContainer(true, false, parentContainer, listName);
			assert(newContainer);
			// All types should only be objects
			assert(newContainer->var.IsObject());
			mTypeRefs[listName] = &newContainer->var.AsObject();
			return;
		}
	PushNewContainer(true, false, parentContainer, listName);
}

vl::JSONLoader::ContainerInfo* vl::JSONLoader::PushNewContainer(bool isObject, bool isList, ContainerInfo* c, const std::string& newContainerName)
{
	if (!isObject && !isList)
		return nullptr;
	auto& v = c->var;
	if (v.IsObject())
	{
		auto& obj = v.AsObject();
		if (isObject)
			obj.Set(mCurrentKey, vl::Object());
		else if (isList)
			obj.Set(mCurrentKey, vl::List());
		return PushContainer(&obj.Get(mCurrentKey.c_str()), newContainerName);
	}
	else if (v.IsList())
	{
		auto& l = v.AsList();
		if (isObject)
			l.Add(vl::Object());
		else if (isList)
			l.Add(vl::List());
		return PushContainer(&l.Back(), newContainerName);
	}
	return nullptr;
}

void vl::JSONLoader::StoreUnresolvedRef(vl::Object& ref)
{
	mUnresolvedRefs.push_back(&ref);
}

void vl::JSONLoader::ResolveRefs()
{
	for (auto it = mUnresolvedRefs.begin(); it != mUnresolvedRefs.end();)
	{
		auto ref = *it;
		auto& typeId = ref->Get("typeid").AsString().Val();
		auto tIt = mTypeRefs.find(typeId);
		if (tIt != mTypeRefs.end())
		{
			*ref = *tIt->second;
			it = mUnresolvedRefs.erase(it);
		}
		else
			++it;
	}
}

// rapidjson interface definition
bool vl::JSONLoader::Null()
{
	auto varPtr = MakePtr();
	if (!AddVar(varPtr))
		return false;
	return true;
}

bool vl::JSONLoader::Bool(bool b)
{
	auto varPtr = MakePtr(b);
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::Int(int i)
{
	auto varPtr = MakePtr(i);
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::Uint(unsigned i)
{
	// TODO: support
	// Now just hard-converted to int
	auto varPtr = MakePtr(int(i));
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::Int64(int64_t i)
{
	// TODO: support
	// Now just hard-converted to int
	auto varPtr = MakePtr(int(i));
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::Uint64(uint64_t i)
{
	// TODO: support
	// Now just hard-converted to int
	auto varPtr = MakePtr(int(i));
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::Double(double d)
{
	// TODO: support
	// Now just hard-converted to float
	auto varPtr = MakePtr(float(d));
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::RawNumber(const Ch* str, SizeType length, bool copy)
{
	auto varPtr = MakePtr(str);
	AddVar(varPtr);
	return true;
}

bool vl::JSONLoader::String(const Ch* str, SizeType length, bool copy)
{
	if (mCurrentProto)
	{
		assert(!mUnresolvedRefs.empty());
		mUnresolvedRefs.back()->Set("typeid", str);
		mCurrentProto = false;
	}
	else
	{
		auto varPtr = MakePtr(str);
		AddVar(varPtr);
	}
	return true;
}

bool vl::JSONLoader::StartObject()
{
	bool isRoot = false;
	auto c = GetCurrentContainer();
	if (c == nullptr)
	{
		PushContainer(&mObject, "_root_");
		isRoot = true;
	}
	else
	{
		if (c->var.IsObject())
			if (mCurrentKey.empty())
				LOG_WARNING("Empty key with an object body encountered in container '" << c->name << "'");
		PushNewObject(c, mCurrentKey);
	}
	return true;
}

bool vl::JSONLoader::Key(const Ch* str, SizeType length, bool copy)
{
	auto currentContainer = GetCurrentContainer();
	assert(currentContainer);
	assert(currentContainer->var.IsObject());
	if (std::strcmp(str, "proto") == 0)
	{
		StoreUnresolvedRef(currentContainer->var.AsObject().Set(str, vl::Object()).AsObject());
		mCurrentProto = true;
	}
	else
		currentContainer->var.AsObject().Set(str);
	mCurrentKey = str;
	mKeyProcessed = false;
	return true;
}

bool vl::JSONLoader::EndObject(SizeType memberCount)
{
	PopContainer();
	// TODO: try to do it once in the end of the parsing process
	ResolveRefs();
	return true;
}

bool vl::JSONLoader::StartArray()
{
	bool isRoot = false;
	auto c = GetCurrentContainer();
	if (c == nullptr)
	{
		// Root node should be an object
		return false;
	}
	PushNewList(c, mCurrentKey);
	return true;
}

bool vl::JSONLoader::EndArray(SizeType elementCount)
{
	PopContainer();
	return true;
}

