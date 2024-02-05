#pragma once

#include <cstddef>
#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "vl_fwd.h"
#include <utils/patterns/Observer.h>
#include <utils/patterns/Observable.h>
#include <vl/var_ptr.h>

namespace vl
{
	// AbstractVar
	// Make it possible to store a list of various variables
	class AbstractVar : public VarInterface
	{
	public:
		const BoolVar& AsBool() const override;
		const NumberVar& AsNumber() const override;
		const StringVar& AsString() const override;
		const ObjectVar& AsObject() const override;
		const ListVar& AsList() const override;
		BoolVar& AsBool() override;
		NumberVar& AsNumber() override;
		StringVar& AsString() override;
		ObjectVar& AsObject() override;
		ListVar& AsList() override;
		bool IsBool() const override { return false; }
		bool IsNumber() const override { return false; }
		bool IsString() const override { return false; }
		bool IsObject() const override { return false; }
		bool IsList() const override { return false; }
		bool IsNull() const override { return true; }
		virtual bool Accept(Visitor& v, const char* name = nullptr) const { return true; }
		Type GetType() const override;
		virtual VarPtr Ptr() const = 0;
		std::string ToStr() const override { return ""; }
		const void* Data() const override;
		vl::VarPtr Copy() const override;
		const VarPtr& operator[](const char* s) const override;
		VarPtr& operator[](const char* s) override;

	protected:
		template <typename T>
		VarPtr PtrImpl(const T* body) const {
			auto p = std::make_shared<T>();
			*(p.get()) = *body;
			return std::dynamic_pointer_cast<AbstractVar>(p);
		}
	};

	// Return proto name if assigned
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


	// ======= Concrete Vars =======
	// BoolVar declaration
	// Non sharable
	class BoolVar : public AbstractVar
	{
	public:
		BoolVar() = default;
		BoolVar(bool value) : mData(value) {}
		bool IsBool() const override { return true; }
		const BoolVar& AsBool() const override { return *this; }
		BoolVar& AsBool() override { return *this; }
		Type GetType() const override;
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::string ToStr() const override;
		BoolVar& operator=(bool val);
		
	private:
		bool mData = false;
	};

	// NumberVar declaration
	// Non sharable
	class NumberVar : public AbstractVar
	{
	public:
		NumberVar() = default;
		NumberVar(float value) : mData(value) {}
		bool IsNumber() const override { return true; }
		const NumberVar& AsNumber() const override { return *this; }
		NumberVar& AsNumber() override { return *this; }
		Type GetType() const override;
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		float Val() const { return mData; }
		std::string ToStr() const override;
		NumberVar& operator=(int val);
		NumberVar& operator=(double val);
		NumberVar& operator=(float val);

	private:
		float mData = 0.f;
	};

	// StringVar declaration
	// Non sharable
	class StringVar : public AbstractVar
	{
	public:
		StringVar() = default;
		StringVar(const std::string& value) : mData(value) {}
		bool IsString() const override { return true; }
		const StringVar& AsString() const override { return *this; }
		StringVar& AsString() override { return *this; }
		Type GetType() const override;
		VarPtr Ptr() const override { return PtrImpl(this); }
		const std::string& Val() const { return mData; }
		bool IsNull() const override { return false; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::string ToStr() const override;
		StringVar& operator=(const std::string& val) {
			mData = val;
			return *this;
		}

	private:
		std::string mData;
	};

	typedef std::unordered_map<std::string, VarPtr> PropsContainerType;

	// Return type of ObjectVar::Set
	// Includes id and data
	// TODO: support
	//struct ObjectSetRet {
	//	bool addition = false
	//	ObjectInsertRet() = default;
	//	ObjectSetRet(bool addition, vl::Var& data)
	//		: addition(addition)
	//		, mData(&data)
	//	{}
	//	vl::Var& data();
	//	operator bool() { return mData != nullptr; }
	//	static ObjectSetRet Null();

	//private:
	//	vl::Var* mData = nullptr;
	//};

	// ObjectVar declaration
	// Sharable
	/// <summary>
	///  Object is both an observable and observer.
	///  It observes its nested objects's changes
	/// </summary>
	/// <param name="info"></param>
	struct PropsDataType : public Observable, public Observer
	{
		void Notify(vl::VarPtr info) override;
		void Update(Observable* sender, vl::VarPtr info = nullptr) override;
		PropsContainerType data;
	};
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
		Type GetType() const override;
		std::size_t Size() const;
		VarPtr& Set(const std::string& propName);
		VarPtr& Set(const std::string& propName, const Var& value);
		VarPtr& Set(const std::string& propName, const VarPtr& varPtr);
		template <typename T>
		VarPtr& Set(const std::string& propName, const T& value)
		{
			return Set(propName, MakePtr(value));
		}
		const VarPtr& operator[](const char* s) const override;
		VarPtr& operator[](const char* s) override;
		const VarPtr& Get(const std::string& propName) const;
		VarPtr& Get(const std::string& propName);
		bool Has(const std::string& propName) const;
		bool HasOwn(const std::string& propName) const;
		std::shared_ptr<std::string> GetRelativePath(const std::string& propName) const;
		bool Overridden(const std::string& propName) const;
		std::size_t PropCount() const;
		bool RemoveProperty(const std::string& propName);
		bool RenameProperty(const std::string& propName, const std::string& newName);
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool IsEmpty() const { return mData == nullptr || mData->data.empty();}
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		vl::VarPtr Copy() const override;
		bool ForeachProp(const std::function<bool(const std::string&, const vl::Var&)>& pred, bool recursive = false) const;
		bool ForeachProp(const std::function<bool(const std::string&, vl::Var&)>& pred, bool recursive = false);
		void SetPrototype(const vl::Object& proto);
		Object& GetPrototype() const;
		std::string ToStr() const override;
		void Attach(Observer* o, const std::string& title = "") {
			mData->Attach(o, title);
		}
		void Detach(Observer* o) {
			mData->Detach(o);
		}
		const void* Data() const override {
			return mData.get();
		}

		Observable* GetObservable() {
			return mData.get();
		}
		void Clear(bool recursive = false);
	protected:
		std::shared_ptr<std::string> getRelativePathRecursive(const std::string& propName, const std::string& path = "") const;
		bool overriddenRecursive(const std::string& propName, int count = 0) const;

	protected:
		ObjectDataType mData = std::make_shared<PropsDataType>();
	};

	extern vl::Object& NullObject();
	
	// Empty var used to return it by reference to some functions
	extern vl::NullVar& EmptyVar();

	// ListVar declaration
	// Sharable
	struct ListDataType : public Observable, public Observer
	{
		void Notify(vl::VarPtr info) override;
		void Update(Observable* sender, vl::VarPtr info = nullptr) override;
		std::vector<VarPtr> data;
	};
	typedef std::shared_ptr<ListDataType> ListVarDataType;

	// Return type of ListVar::Add and Set operations
	// Includes index and data
	struct ListInsertRet {
		int index = -1;
		ListInsertRet() = default;
		ListInsertRet(int index, vl::Var& data)
			: index(index)
			, mData(&data)
		{}
		vl::Var& data();
		operator bool() { return index >= 0; }
		static ListInsertRet Null();
	private:
		vl::Var* mData = nullptr;
	};

	class ListVar : public AbstractVar
	{
	public:
		bool IsList() const override { return true; }
		const ListVar& AsList() const override { return *this; }
		ListVar& AsList() override { return *this; }
		Type GetType() const override;
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool IsNull() const override { return mData == nullptr; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::size_t Size() const {
			return mData ? mData->data.size() : 0;
		}
		void Clear(bool recursive = false);
		bool Remove(int index);
		const Var& At(int index) const;
		Var& At(int index);
		ListInsertRet Add(const VarPtr& varPtr, int indexBefore = -1);
		template <typename T>
		ListInsertRet Add(const T& value, int indexBefore = -1)
		{
			return Add(MakePtr(value), indexBefore);
		}
		ListInsertRet Set(int index);
		ListInsertRet Set(int index, const Var& value);
		ListInsertRet Set(int index, const VarPtr& varPtr);
		template <typename T>
		ListInsertRet Set(int index, const T& value)
		{
			return Set(index, MakePtr(value));
		}
		Var& Back();
		bool IsEmpty() const;
		std::string ToStr() const override;
		void Attach(Observer* o, const std::string& title = "") {
			mData->Attach(o);
		}
		void Detach(Observer* o) {
			mData->Detach(o);
		}
		const void* Data() const override {
			return mData.get();
		}
		vl::VarPtr Copy() const override;

	private:
		ListVarDataType mData = std::make_shared<ListDataType>();
	};

	extern vl::ListVar& EmptyList();

	// NullVar declaration
	// Non sharable
	class NullVar : public AbstractVar
	{
	public:
		bool IsNull() const override { return true; }
		Type GetType() const override;
		VarPtr Ptr() const override { return PtrImpl(this); }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		const void* Data() const override {
			return nullptr;
		}
	};

	
}
