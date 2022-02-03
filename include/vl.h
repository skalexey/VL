#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "vl_fwd.h"

namespace vl
{
	// Return proto name if assigned
	// TODO: pass at least a model as a context
	std::string GetTypeId(const vl::Object& obj, const vl::Object& context);

	// Polymorphic variable (pointer) creation with any supported type
	VarPtr MakePtr(bool value);
	VarPtr MakePtr(int value);
	VarPtr MakePtr(float value);
	VarPtr MakePtr(const char* value);
	VarPtr MakePtr(const std::string& value);
	VarPtr MakePtr(const ObjectVar& value);
	VarPtr MakePtr(const ListVar& value);
	VarPtr MakePtr(const NullVar& value);
	VarPtr MakePtr(const Var& value);
	VarPtr MakePtr(); // Return a null var

	// AbstractVar
	// Make it possible to store a list of various variables
	class AbstractVar
	{
	public:
		virtual const BoolVar& AsBool() const;
		virtual const NumberVar& AsNumber() const;
		virtual const StringVar& AsString() const;
		virtual const ObjectVar& AsObject() const;
		virtual const ListVar& AsList() const;
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
		virtual std::string ToStr() const { return ""; }

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
		const BoolVar& AsBool() const override { return *this; }
		BoolVar& AsBool() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		std::string ToStr() const override;
		BoolVar& operator=(bool val);

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
		const NumberVar& AsNumber() const override { return *this; }
		NumberVar& AsNumber() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		float Val() const { return mData; }
		std::string ToStr() const override;
		NumberVar& operator=(int val);
		NumberVar& operator=(double val);
		NumberVar& operator=(float val);

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
		const StringVar& AsString() const override { return *this; }
		StringVar& AsString() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		const std::string& Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		std::string ToStr() const override;
		StringVar& operator=(const std::string& val) {
			mData = val;
			return *this;
		}
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
		ObjectVar() = default;
		ObjectVar(const ObjectDataType& dataPtr);
		bool IsObject() const override { return true; }
		bool operator == (const ObjectVar& right) const;
		bool operator == (const ObjectVar& right);
		operator bool() const override;
		const ObjectVar& AsObject() const override;
		ObjectVar& AsObject() override;
		int Size() const;
		Var& Set(const std::string& propName);
		Var& Set(const std::string& propName, const Var& value);
		Var& Set(const std::string& propName, const VarPtr& varPtr);
		template <typename T>
		Var& Set(const std::string& propName, const T& value)
		{
			return Set(propName, MakePtr(value));
		}
		const Var& Get(const std::string& propName) const;
		Var& Get(const std::string& propName);
		bool Has(const std::string& propName) const;
		int PropCount() const;
		bool RemoveProperty(const std::string& propName);
		bool RenameProperty(const std::string& propName, const std::string& newName);
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		ObjectVar Copy() const;
		bool ForeachProp(const std::function<bool(const std::string&, const vl::Var&)>& pred, bool recursive = false) const;
		bool ForeachProp(const std::function<bool(const std::string&, vl::Var&)>& pred, bool recursive = false);
		void SetPrototype(const vl::Object& proto);
		Object& GetPrototype() const;
		std::string ToStr() const override;

	protected:
		ObjectDataType mData = std::make_shared<PropsDataType>();
	};

	extern vl::Object nullObject;
	
	// ListVar
	// Sharable
	typedef std::vector<VarPtr> ListDataType;
	typedef std::shared_ptr<ListDataType> ListVarDataType;
	class ListVar : public AbstractVar
	{
	public:
		bool IsList() const override { return true; }
		const ListVar& AsList() const override { return *this; }
		ListVar& AsList() override { return *this; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool Accept(Visitor& v, const char* name = nullptr) override;
		int Size() const;
		void Clear();
		bool Remove(int index);
		const Var& At(int index) const;
		Var& At(int index);
		Var& Add(const VarPtr& varPtr, int indexBefore = -1);
		template <typename T>
		Var& Add(const T& value, int indexBefore = -1)
		{
			return Add(MakePtr(value), indexBefore);
		}
		Var& Set(int index);
		Var& Set(int index, const Var& value);
		Var& Set(int index, const VarPtr& varPtr);
		template <typename T>
		Var& Set(int index, const T& value)
		{
			return Set(index, MakePtr(value));
		}
		Var& Back();
		bool IsEmpty() const;
		ListVar Copy() const;
		std::string ToStr() const override;

	private:
		ListVarDataType mData = std::make_shared<ListDataType>();
	};

	extern vl::ListVar emptyList;

	// NullVar
	// Non sharable
	class NullVar : public AbstractVar
	{
	public:
		bool IsNull() const override { return true; }
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Accept(Visitor& v, const char* name = nullptr) override;
	};

	// Empty var used to return it by reference to some functions
	extern vl::NullVar emptyVar;
}
