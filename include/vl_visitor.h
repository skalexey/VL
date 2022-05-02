#include "vl_fwd.h"

namespace vl
{
	class Visitor
	{
	public:
		virtual bool VisitNull(const NullVar& var, const char* name) { return true; }
		virtual bool VisitBool(const BoolVar& var, const char* name) { return true; }
		virtual bool VisitNumber(const NumberVar& var, const char* name) { return true; }
		virtual bool VisitString(const StringVar& var, const char* name) { return true; }
		virtual bool VisitObject(const ObjectVar& var, const char* name) { return true; }
		virtual bool EndVisitObject(const ObjectVar& var, const char* name) { return true; }
		virtual bool VisitList(const ListVar& var, const char* name) { return true; }
		virtual bool EndVisitList(const ListVar& var, const char* name) { return true; }
	};
}
