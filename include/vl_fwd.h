#pragma once

#include <memory>
#include <functional>

namespace vl
{
	enum class Type
	{
		Bool,
		Number,
		String,
		Pointer,
		Object,
		List,
		Null,
		Count
	};

	class BoolVar;
	class NumberVar;
	class StringVar;
	class PointerVar;
	class ObjectVar;
	class ListVar;
	class NullVar;
	class AbstractVar;
	class Visitor;
	class ObjectSetRet;

	class Observable;
	class Observer;

	// User-friendly type names. Available from the namespace vl
	// Like vl::Bool
	typedef BoolVar Bool;
	typedef NumberVar Number;
	typedef StringVar String;
	typedef PointerVar Pointer;
	typedef ObjectVar Object;
	typedef ListVar List;
	typedef AbstractVar Var;
	typedef NullVar Null;
	class VarPtr;

	extern vl::Object& NullObject();
	extern vl::ListVar& EmptyList();
	extern vl::NullVar& EmptyVar();

	typedef std::function<void(const vl::Object&)> VoidCbObject;
	typedef std::function<bool(const vl::Object&)> BoolCbObject;

		// Return proto name if assigned
	// Polymorphic variable (pointer) creation with any supported type
	VarPtr MakePtr(bool value);
	VarPtr MakePtr(int value);
	VarPtr MakePtr(float value);
	VarPtr MakePtr(const char* value);
	VarPtr MakePtr(const std::string& value);
	VarPtr MakePtr(const void* value);
	VarPtr MakePtr(const ObjectVar& value);
	VarPtr MakePtr(const ListVar& value);
	VarPtr MakePtr(const NullVar& value);
	VarPtr MakePtr(const Var& value);
	VarPtr MakePtr(); // Return a null var
}
