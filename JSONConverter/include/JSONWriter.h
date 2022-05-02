#include "vl_visitor.h"
#include "JSONDefs.h"
#include "rapidjson/fwd.h"
#include "rapidjson/document.h"
#include "TypeResolver.h"

namespace vl
{
	class JSONWriter : public Visitor
	{
	public:
		JSONWriter(const TypeResolver& typeResolver, const CnvParams& params = CnvParams());

		bool VisitNull(const NullVar& var, const char* name) override;
		bool VisitBool(const BoolVar& var, const char* name) override;
		bool VisitNumber(const NumberVar& var, const char* name) override;
		bool VisitString(const StringVar& var, const char* name) override;
		bool VisitObject(const ObjectVar& var, const char* name) override;
		bool EndVisitObject(const ObjectVar& var, const char* name) override;
		bool VisitList(const ListVar& var, const char* name) override;
		bool EndVisitList(const ListVar& var, const char* name) override;
		rapidjson::Document& GetDocument() { return mDoc; }

	protected:
		void AddMember(rapidjson::Value& val, const char* name);
		bool AddProto(const ObjectVar& value);
		rapidjson::Value* GetCurrentContainer();
		void PushCurrentContainer(rapidjson::Value* val);
		bool PopCurrentContainer();

	private:
		rapidjson::Document mDoc;
		std::vector<rapidjson::Value*> mStack;
		vl::CnvParams mCnvParams;
		// TODO: Check for potential memory hold
		TypeResolver mTypeResolver;
	};
}
