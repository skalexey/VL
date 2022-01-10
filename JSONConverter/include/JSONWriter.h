#include "visitor.h"
#include "JSONDefs.h"
#include "rapidjson/fwd.h"
#include "rapidjson/document.h"

namespace vl
{
	class JSONWriter : public Visitor
	{
	public:
		JSONWriter(const vl::Object& context, const CnvParams& params = CnvParams());

		bool VisitNull(NullVar& var, const char* name) override;
		bool VisitBool(BoolVar& var, const char* name) override;
		bool VisitNumber(NumberVar& var, const char* name) override;
		bool VisitString(StringVar& var, const char* name) override;
		bool VisitObject(ObjectVar& var, const char* name) override;
		bool EndVisitObject(ObjectVar& var, const char* name) override;
		bool VisitList(ListVar& var, const char* name) override;
		bool EndVisitList(ListVar& var, const char* name) override;
		rapidjson::Document& GetDocument() { return mDoc; }

	protected:
		void AddMember(rapidjson::Value& val, const char* name);
		bool AddProto(ObjectVar& value);
		rapidjson::Value* GetCurrentContainer();
		void PushCurrentContainer(rapidjson::Value* val);
		bool PopCurrentContainer();

	private:
		rapidjson::Document mDoc;
		std::vector<rapidjson::Value*> mStack;
		vl::CnvParams mCnvParams;
		// TODO: Check for potential memory hold
		vl::Object mContext;
	};
}
