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
		Object,
		List,
		Null,
		Count
	};

	class BoolVar;
	class NumberVar;
	class StringVar;
	class ObjectVar;
	class ListVar;
	class NullVar;
	class AbstractVar;
	class Visitor;
	class ObjectSetRet;

	// User-friendly type names. Available from the namespace vl
	// Like vl::Bool
	typedef BoolVar Bool;
	typedef NumberVar Number;
	typedef StringVar String;
	typedef ObjectVar Object;
	typedef ListVar List;
	typedef AbstractVar Var;
	typedef NullVar Null;
	typedef std::shared_ptr<Var> VarPtr;

	extern vl::Object nullObject;
	extern vl::ListVar emptyList;
	extern vl::NullVar emptyVar;

	typedef std::function<void(const vl::Object&)> VoidCbObject;
	typedef std::function<bool(const vl::Object&)> BoolCbObject;
}
