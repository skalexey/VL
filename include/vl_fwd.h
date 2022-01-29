#pragma once

#include <memory>

namespace vl
{
	enum class Type
	{
		Bool,
		Number,
		String,
		Object,
		List,
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
}