#include "vl/var_ptr.h"
#include "VL.h"

namespace vl
{
	// ======= Begin of VarPtr definitions =======
	bool VarPtr::operator==(const VarPtr& right)
	{
		return *mPtr == *right.mPtr;
	}
	
	const BoolVar& VarPtr::AsBool() const
	{
		return mPtr->AsBool();
	}
	
	const NumberVar& VarPtr::AsNumber() const
	{
		return mPtr->AsNumber();
	}
	
	const StringVar& VarPtr::AsString() const
	{
		return mPtr->AsString();
	}
	
	const ObjectVar& VarPtr::AsObject() const
	{
		return mPtr->AsObject();
	}
	
	const ListVar& VarPtr::AsList() const
	{
		return mPtr->AsList();
	}
	
	BoolVar& VarPtr::AsBool()
	{
		return mPtr->AsBool();
	}

	NumberVar& VarPtr::AsNumber()
	{
		return mPtr->AsNumber();
	}

	StringVar& VarPtr::AsString()
	{
		return mPtr->AsString();
	}
	
	ObjectVar& VarPtr::AsObject()
	{
		return mPtr->AsObject();
	}

	ListVar& VarPtr::AsList()
	{
		return mPtr->AsList();
	}

	bool VarPtr::IsNull() const
	{
		return mPtr ? mPtr->IsNull() : true;
	}
	
	bool VarPtr::IsBool() const
	{
		return mPtr ? mPtr->IsBool() : false;
	}
	
	bool VarPtr::IsNumber() const
	{
		return mPtr ? mPtr->IsNumber() : false;
	}

	bool VarPtr::IsString() const
	{
		return mPtr ? mPtr->IsString() : false;
	}
	
	bool VarPtr::IsObject() const
	{
		return mPtr ? mPtr->IsObject() : false;
	}

	bool VarPtr::IsList() const
	{
		return mPtr ? mPtr->IsList() : false;
	}

	Type VarPtr::GetType() const
	{
		return mPtr->GetType();
	}

	VarPtr::operator bool() const
	{
		return mPtr ? (*mPtr).operator bool() : false;
	}
	
	std::string VarPtr::ToStr() const
	{
		return mPtr ? mPtr->ToStr() : "null";
	}

	const void* VarPtr::Data() const
	{
		return mPtr ? mPtr->Data() : nullptr;
	}

	vl::VarPtr VarPtr::Copy() const
	{
		return mPtr->Copy();
	}

	const VarPtr& VarPtr::operator[](const char* s) const
	{
		return (*mPtr).operator[](s);
	}

	VarPtr& VarPtr::operator[](const char* s)
	{
		return (*mPtr).operator[](s);
	}
	// ======= End of VarPtr definitions =======
}
