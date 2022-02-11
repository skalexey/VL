// This is a custom visitor for rapidjson::Value.
// The interface described by the link below:
// https://internal.dunescience.org/doxygen/classrapidjson_1_1Handler.html

#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <unordered_map>
#include "vl_fwd.h"

namespace vl
{
	class JSONLoader
	{
		struct ContainerInfo;

	public:
		JSONLoader(vl::Object& object) : mObject(object) {}
		ContainerInfo* GetCurrentContainer();
		ContainerInfo* PushContainer(vl::Var* ptr, const std::string& name);
		void PopContainer();

		// rapidjson interface
		typedef char Ch;
		typedef size_t SizeType;
		bool Null();
		bool Bool(bool b);
		bool Int(int i);
		bool Uint(unsigned i);
		bool Int64(int64_t i);
		bool Uint64(uint64_t i);
		bool Double(double d);
		/// enabled via kParseNumbersAsStringsFlag, string is not null-terminated (use length)
		bool RawNumber(const Ch* str, SizeType length, bool copy);
		bool String(const Ch* str, SizeType length, bool copy);
		bool StartObject();
		bool Key(const Ch* str, SizeType length, bool copy);
		bool EndObject(SizeType memberCount);
		bool StartArray();
		bool EndArray(SizeType elementCount);

	protected:
		bool AddVar(const vl::VarPtr& ptr);
		// 3 following functions: Memorize current object or a list as a container for the next parsed elements
		void PushNewList(ContainerInfo* parentContainer, const std::string& listName);
		void PushNewObject(ContainerInfo* c, const std::string& objectName);
		ContainerInfo* PushNewContainer(
			bool isObject
			, bool isList
			, ContainerInfo* parentConainer
			, const std::string& newContainerName
		);
		// Store references to prototypes which types have not been parsed yet
		void StoreUnresolvedRef(vl::Object& ref);
		// Link eventually parsed type objects to the prototypes
		void ResolveRefs();

	private:
		struct ContainerInfo
		{
			ContainerInfo(vl::Var& containerInstance, const std::string& containerName)
				: var(containerInstance)
				, name(containerName)
			{}
			vl::Var& var;
			std::string name;
		};
	private:
		vl::Object& mObject;
		std::vector<ContainerInfo> mStack;
		std::string mCurrentKey;
		bool mKeyProcessed = true;
		bool mCurrentProto = false;
		std::list<vl::Object*> mUnresolvedRefs;
		std::unordered_map<std::string, vl::Object*> mTypeRefs;
	};
}
