#include "vl/var_ptr.h"
#include "vl.h"

namespace vl
{
	// ======= Begin of VarPtr definitions =======
	VarPtr::VarPtr(const char* str)
		: mPtr(std::make_shared<StringVar>(str))
	{}

	VarPtr::VarPtr(const std::string& str)
		: mPtr(std::make_shared<StringVar>(str))
	{}

	VarPtr::VarPtr(bool b)
		: mPtr(std::make_shared<BoolVar>(b))
	{}

	VarPtr::VarPtr(int i)
		: mPtr(std::make_shared<NumberVar>(i))
	{}

	VarPtr::VarPtr(const void* ptr)
		: mPtr(std::make_shared<PointerVar>(ptr))
	{}

	const utils::entity* VarPtr::self() const
	{
		return mPtr.get();
	}

	bool VarPtr::operator==(const VarPtr& right)
	{
		return *mPtr == *right.mPtr;
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
		static VarPtr emptyVar;
		if (is<Object>())
			return as<Object>()[s];
		return emptyVar;
	}

	VarPtr& VarPtr::operator[](const char* s)
	{
		return const_cast<VarPtr&>(static_cast<const VarPtr&>(*this)[s]);
	}

	bool VarPtr::Same(const VarInterface& other) const
	{
		return mPtr->Same(other);
	}
	// ======= End of VarPtr definitions =======
}
