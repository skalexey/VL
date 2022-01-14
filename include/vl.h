#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
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

	// Polymorphic variable (pointer) creation with any supported type
	VarPtr MakePtr(bool value);
	VarPtr MakePtr(int value);
	VarPtr MakePtr(float value);
	VarPtr MakePtr(const char* value);
	VarPtr MakePtr(const std::string& value);
	VarPtr MakePtr(const ObjectVar& value);
	VarPtr MakePtr(const ListVar& value);
	VarPtr MakePtr(const NullVar& value);
	VarPtr MakePtr(Var& value);
	VarPtr MakePtr(); // Return a null var

	// AbstractVar
	// Make it possible to store a list of various variables
	class AbstractVar
	{
	public:
		virtual BoolVar& AsBool();
		virtual NumberVar& AsNumber();
		virtual StringVar& AsString();
		virtual ObjectVar& AsObject();
		virtual ListVar& AsList();
		virtual bool IsBool() const { return false; }
		virtual bool IsNumber() const { return false; }
		virtual bool IsString() const { return false; }
		virtual bool IsObject() const { return false; }
		virtual bool IsList() const { return false; }
		virtual bool IsNull() const { return true; }
		virtual VarPtr Ptr() const = 0;
		virtual bool Accept(Visitor& v, const char* name = nullptr) { return true; }
		virtual operator bool() const { return !IsNull(); }

	protected:
		template <typename T>
		VarPtr PtrImpl(const T* body) const {
			auto p = std::make_shared<T>();
			*(p.get()) = *body;
			return std::dynamic_pointer_cast<AbstractVar>(p);
		}
	};

	// ======= Concrete Vars =======
	// BoolVar
	// Non sharable
	class BoolVar : public AbstractVar
	{
	public:
		BoolVar() = default;
		BoolVar(bool value) : mData(value) {}
		bool IsBool() const override { return true; }
		BoolVar& AsBool() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;

	private:
		bool mData = false;
	};

	// NumberVar
	// Non sharable
	class NumberVar : public AbstractVar
	{
	public:
		NumberVar() = default;
		NumberVar(float value) : mData(value) {}
		bool IsNumber() const override { return true; }
		NumberVar& AsNumber() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		float Val() const { return mData; }

	private:
		float mData = 0.f;
	};

	// StringVar
	// Non sharable
	class StringVar : public AbstractVar
	{
	public:
		StringVar() = default;
		StringVar(const std::string& value) : mData(value) {}
		bool IsString() const override { return true; }
		StringVar& AsString() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		const std::string& Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;

	private:
		std::string mData;
	};

	// ObjectVar
	// Sharable
	typedef std::unordered_map<std::string, VarPtr> PropsDataType;
	typedef std::shared_ptr<PropsDataType> ObjectDataType;
	class ObjectVar : public AbstractVar
	{
	public:
		bool IsObject() const override { return true; }
		bool operator == (const ObjectVar& right);
		ObjectVar& AsObject() override { return *this; }
		Var& Set(const std::string& propName);
		Var& Set(const std::string& propName, Var& value);
		Var& Set(const std::string& propName, const VarPtr& varPtr);
		template <typename T>
		Var& Set(const std::string& propName, const T& value)
		{
			assert(mData);
			VarPtr var = MakePtr(value);
			return *((*mData)[propName] = var);
		}
		Var& Get(const std::string& propName);
		bool Has(const std::string& propName);
		bool RemoveProperty(const std::string& propName);
		bool RenameProperty(const std::string& propName, const std::string& newName);
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		ObjectVar Copy() const;
		bool ForeachProp(const std::function<bool(const std::string&, vl::Var&)>& pred);
		void SetPrototype(const vl::Object& proto);

	protected:
		ObjectDataType mData = std::make_shared<PropsDataType>();
	};
	
	// ListVar
	// Sharable
	typedef std::vector<VarPtr> ListDataType;
	typedef std::shared_ptr<ListDataType> ListVarDataType;
	class ListVar : public AbstractVar
	{
	public:
		bool IsList() const override { return true; }
		ListVar& AsList() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		Var& Add(const VarPtr& varPtr);
		template <typename T>
		Var& Add(const T& value)
		{
			assert(mData);
			VarPtr var = MakePtr(value);
			mData->push_back(var);
			return *mData->back();
		}
		Var& Back();
		bool IsEmpty() const;
		ListVar Copy() const;

	private:
		ListVarDataType mData = std::make_shared<ListDataType>();
	};

	// NullVar
	// Non sharable
	class NullVar : public AbstractVar
	{
	public:
		bool IsNull() const override { return true; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Accept(Visitor& v, const char* name = nullptr) override;
	};
}
