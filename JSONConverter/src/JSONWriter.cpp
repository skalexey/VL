#include <cstring>
#include "vl.h"
#include "JSONWriter.h"
#include "rapidjson/document.h"

vl::JSONWriter::JSONWriter(const TypeResolver& typeResolver, const vl::CnvParams& params)
	: mTypeResolver(typeResolver)
	, mCnvParams(params)
{}

void vl::JSONWriter::AddMember(rapidjson::Value& val, const char* name)
{
	bool isRoot = false;
	if (!GetCurrentContainer())
	{
		if (mDoc.GetType() == rapidjson::kNullType)
		{
			if (val.IsObject())
				mDoc.SetObject();
			else if (val.IsArray())
				mDoc.SetArray();
		}
		isRoot = true;
		PushCurrentContainer(&mDoc);
	}
	auto currentVal = GetCurrentContainer();
	if (currentVal->IsObject())
	{
		if ((val.IsObject() && !isRoot) || !val.IsObject())
		{
			// Create a new rapidjson object
			// If the root object passed then use Document as it's body
			assert(name); // Any property if an object should have it's name
			auto nameStr = (name ? name : "");
			auto it = currentVal->FindMember(nameStr);
			if (it != currentVal->MemberEnd())
				it->value = val;
			else
				currentVal->AddMember(rapidjson::StringRef(name ? name : ""), val, mDoc.GetAllocator());
			auto& newVal = (*currentVal)[name];
			if (newVal.IsObject() || newVal.IsArray())
				PushCurrentContainer(&newVal);
		}
	}
	else if (currentVal->IsArray())
	{
		currentVal->PushBack(val, mDoc.GetAllocator());
		auto& newVal = (*currentVal)[currentVal->Size() - 1];
		if (newVal.IsObject() || newVal.IsArray())
			PushCurrentContainer(&newVal);
	}
	else
		throw "JSONWriter: unsupported container type for adding elements";
}

bool vl::JSONWriter::AddProto(const ObjectVar& value)
{
	auto currentVal = GetCurrentContainer();
	if (mCnvParams.useProtoRefs)
	{
		rapidjson::Value val;
		auto typeId = mTypeResolver.GetTypeId(value);
		val.SetString(typeId.c_str(), mDoc.GetAllocator());
		AddMember(val, "proto");
		return false; // Don't visit nested 'proto' object
	}
	else
	{
		// Put everything from 'proto' in the current container
		PushCurrentContainer(GetCurrentContainer());
		if (mCnvParams.storeTypeId)
		{
			rapidjson::Value val;
			val.SetString(mTypeResolver.GetTypeId(value).c_str(), mDoc.GetAllocator());
			AddMember(val, "typeid");
		}
		return true;
	}
}

rapidjson::Value* vl::JSONWriter::GetCurrentContainer()
{
	return mStack.empty() ? nullptr : mStack.back();
}

void vl::JSONWriter::PushCurrentContainer(rapidjson::Value* val)
{
	mStack.push_back(val);
}

bool vl::JSONWriter::PopCurrentContainer()
{
	if (mStack.empty())
		return false;
	mStack.pop_back();
	return true;
}

bool vl::JSONWriter::VisitNull(const vl::NullVar& var, const char* name)
{
	rapidjson::Value val;
	val.SetNull();
	AddMember(val, name);
	return true;
}

bool vl::JSONWriter::VisitBool(const BoolVar& var, const char* name)
{
	rapidjson::Value val;
	val.SetBool(var.Val());
	AddMember(val, name);
	return true;
}

bool vl::JSONWriter::VisitNumber(const NumberVar& var, const char* name)
{
	rapidjson::Value val;
	val.SetDouble(var.Val());
	AddMember(val, name);
	return true;
}

bool vl::JSONWriter::VisitString(const StringVar& var, const char* name)
{
	rapidjson::Value val;
	val.SetString(var.Val().c_str(), mDoc.GetAllocator());
	AddMember(val, name);
	return true;
}

bool vl::JSONWriter::VisitObject(const ObjectVar& var, const char* name)
{
	if (!var.IsNull())
	{
		if (name && std::strcmp(name, "proto") == 0)
		{
			if (!AddProto(var))
				return false;
		}
		else
		{
			rapidjson::Value val;
			val.SetObject();
			AddMember(val, name);
		}
	}
	else
	{
		auto currentVal = GetCurrentContainer();
		if (!currentVal)
			return false;
		rapidjson::Value val;
		val.SetNull();
		currentVal->AddMember(rapidjson::StringRef(name ? name : ""), val, mDoc.GetAllocator());
	}
	return true;
}

bool vl::JSONWriter::EndVisitObject(const ObjectVar& var, const char* name)
{
	PopCurrentContainer();
	return true;
}

bool vl::JSONWriter::VisitList(const ListVar& var, const char* name)
{
	if (!var.IsNull())
	{
		rapidjson::Value val;
		val.SetArray();
		assert(val.IsArray());
		AddMember(val, name);
	}
	else
	{
		rapidjson::Value val;
		val.SetNull();
		AddMember(val, name);
	}
	return true;
}

bool vl::JSONWriter::EndVisitList(const ListVar& var, const char* name)
{
	PopCurrentContainer();
	return true;
}
