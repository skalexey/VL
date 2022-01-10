namespace vl
{
	class NullVar;
	class BoolVar;
	class NumberVar;
	class StringVar;
	class ObjectVar;
	class ListVar;

	class Visitor
	{
	public:
		virtual bool VisitNull(NullVar& var, const char* name) { return true; }
		virtual bool VisitBool(BoolVar& var, const char* name) { return true; }
		virtual bool VisitNumber(NumberVar& var, const char* name) { return true; }
		virtual bool VisitString(StringVar& var, const char* name) { return true; }
		virtual bool VisitObject(ObjectVar& var, const char* name) { return true; }
		virtual bool EndVisitObject(ObjectVar& var, const char* name) { return true; }
		virtual bool VisitList(ListVar& var, const char* name) { return true; }
		virtual bool EndVisitList(ListVar& var, const char* name) { return true; }
	};
}
