#pragma once

#include <type_traits>
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
		virtual bool Accept(Visitor& v, const char* name = nullptr) const { return true; }
		Type GetType() const override;
		virtual VarPtr Ptr() {
			return const_cast<const AbstractVar&>(*this).Ptr();
		};
		virtual VarPtr Ptr() const = 0;
		std::string ToStr() const override { return ""; }
		const void* Data() const override;
		bool Same(const VarInterface& right) const override { return false; }

	protected:
		template <typename T>
		VarPtr ptrImpl() {
			return const_cast<const AbstractVar&>(*this).getImpl();
		}
		template <typename T>
		VarPtr ptrImpl() const {
			if (!weak_from_this().expired()) {
				auto ptr = std::dynamic_pointer_cast<const T>(shared_from_this()); // Entity to T
				return std::const_pointer_cast<T>(ptr);
			} else
				return MakePtr(static_cast<const T&>(*this));
		}
		template <typename T>
		VarPtr shallowCopy() const {
			auto p = std::make_shared<T>();
			*(p.get()) = static_cast<const T&>(*this);
			return std::dynamic_pointer_cast<AbstractVar>(p);
		}
	};

	// ======= Concrete Vars =======
	// BoolVar declaration
	// Non sharable
	class BoolVar : public AbstractVar
	{
	public:
		BoolVar() = default;
		BoolVar(bool value) : mData(value) {}
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<BoolVar>(); }
		VarPtr CopyAsPtr() const override { return shallowCopy<BoolVar>(); }
		bool Val() const { return mData; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::string ToStr() const override;
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
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
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<NumberVar>(); }
		VarPtr CopyAsPtr() const override { return shallowCopy<NumberVar>(); }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		template <typename T = float>
		T Val() const {
			return static_cast<T>(mData);
		}
		std::string ToStr() const override;
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
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
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<StringVar>(); }
		VarPtr CopyAsPtr() const override { return shallowCopy<StringVar>(); }
		const std::string& Val() const { return mData; }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::string ToStr() const override;
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
		StringVar& operator=(const std::string& val) {
			mData = val;
			return *this;
		}

	private:
		std::string mData;
	};

	// PointerVar declaration
	// Non sharable
	class PointerVar : public AbstractVar
	{
	public:
		PointerVar() = default;
		PointerVar(const void* value) : mData((void*)value) {}
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<PointerVar>(); }
		VarPtr CopyAsPtr() const override { return shallowCopy<PointerVar>(); }
		template <typename T = void>
		const T* GetVal() const { return reinterpret_cast<const T*>(mData); }
		template <typename T = void>
		T* Val() const { return reinterpret_cast<T*>(mData); }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::string ToStr() const override;
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
		PointerVar& operator=(const void* val) {
			mData = (void*)val;
			return *this;
		}
		bool IsNull() const override {
			return mData == nullptr;
		}

	private:
		void* mData = nullptr;
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
		ObjectVar(std::nullptr_t null_ptr)
		: mData(nullptr) {}
		bool operator==(const VarInterface& right) const;
		bool operator==(const VarInterface& right);
		bool Same(const VarInterface& right) const override;
		operator bool() const override;
		Type GetType() const override;
		std::size_t Size() const;
		VarPtr& Set(const std::string& propName);
		VarPtr& Set(const std::string& propName, const VarPtr& varPtr);
		template <typename T>
		VarPtr& Set(const std::string& propName, const T& value) {
			if constexpr (std::is_same_v<T, VarPtr>)
				return Set(propName, static_cast<const VarPtr&>(value));
			else
				return Set(propName, static_cast<const VarPtr&>(MakePtr(value)));
		}
		// Allow templating subscript operator for reusing in other methods.
		template <typename T = VarPtr>
		const T& operator[](const char* s) const {
			return Get<T>(s);
		}
		template <typename T = VarPtr>
		const T& operator[](const std::string& s) const
		{
			return Get<T>(s);
		}
		template <typename T = VarPtr>
		T& operator[](const char* s) {
			if (auto ptr = getImpl<T>(s))
				return const_cast<T&>(*ptr);
			if constexpr (std::is_same_v<T, VarPtr>)
				return Set(s, T());
			else
				return Set(s, VarPtr::Make<T>()).as<T>();
		}
		template <typename T = VarPtr>
		T& operator[](const std::string& s) {
			return operator[]<T>(s.c_str());
		}
		// Returns reference to an existing variable stored under <propName>. If the value does not exist, it will crash on dereferencing nullptr.
		template <typename T = VarPtr>
		const T& Get(const std::string& propName) const {
			return *getImpl<T>(propName);
		}
		template <typename T = VarPtr>
		T& Get(const std::string& propName)
		{
			return const_cast<T&>(const_cast<const ObjectVar*>(this)->Get<T>(propName));
		}
		// Get() with default value. Always returns a new object (not a reference)
		template <typename T = VarPtr, typename Default_t = VarPtr>
		T Get(const std::string& propName, Default_t def) const {
			if (mData)
				if (auto result = getImpl<T>(propName))
					return *result;
			return def;
		}
		// Always returns a value. Creates a new one if it does not exist.
		template <typename T = VarPtr>
		T& Def(const std::string& propName) {
			return operator []<T>(propName);
		}
		// Const version of Def(). Always returns a value, but can't create a new one, so it returns a new default-initialized instance of T if it does not exist.
		template <typename T = VarPtr>
		const T& GetDef(const std::string& propName) const {
			if (auto ptr = getImpl<T>(propName))
				return *ptr;
			static T emptyValue;
			return emptyValue;
		}
		bool Has(const std::string& propName) const;
		bool HasOwn(const std::string& propName) const;
		std::shared_ptr<std::string> GetRelativePath(const std::string& propName) const;
		bool Overridden(const std::string& propName) const;
		std::size_t PropCount() const;
		bool RemoveProperty(const std::string& propName);
		bool RenameProperty(const std::string& propName, const std::string& newName);
		VarPtr Ptr() const override { return ptrImpl<ObjectVar>(); }
		bool IsNull() const override {
			return mData == nullptr;
		}
		bool IsEmpty() const { return mData == nullptr || mData->data.empty();}
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		vl::VarPtr CopyAsPtr() const override;
		vl::ObjectVar Copy() const;
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
		template <typename T = VarPtr>
		const T* getImpl(const std::string& propName) const {
			auto it = mData->data.find(propName);
			if (it != mData->data.end())
				// VarPtr redirects self() to its mPtr
				if (auto ptr = dynamic_cast<const T*>(&it->second))
					return ptr;
				else if (auto& ptr = it->second)
					return ptr->template as_raw_ptr<T>();
				else
					if (auto& proto = GetPrototype())
						if (auto proto_search_result = proto.Get(propName, VarPtr()))
							return proto_search_result.as_raw_ptr<T>();
			return nullptr;
		}

	protected:
		ObjectDataType mData = std::make_shared<PropsDataType>();
	};

	extern vl::Object& NullObject();
	
	// Empty var used to return it by reference to some functions
	extern vl::NullVar& EmptyVar();
	
	extern vl::VarPtr& EmptyVarPtr();

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
		ListVar() = default;
		template <typename T>
		ListVar(const std::vector<T>& data) {
			for (const auto& d : data)
				Add(d);
		}
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<ListVar>(); }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		std::size_t Size() const {
			return mData ? mData->data.size() : 0;
		}
		void Clear(bool recursive = false);
		bool Remove(int index);
		const VarPtr& At(int index) const;
		VarPtr& At(int index);
		VarPtr& operator[](int index) {
			return At(index);
		}
		const VarPtr& operator[](int index) const {
			return At(index);
		}
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
		vl::VarPtr CopyAsPtr() const override;
		vl::ListVar Copy() const;
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
		bool IsNull() const {
			return mData == nullptr;
		}
		
	private:
		ListVarDataType mData = std::make_shared<ListDataType>();
	};

	extern vl::ListVar& EmptyList();

	// NullVar declaration
	// Non sharable
	class NullVar : public AbstractVar
	{
	public:
		bool IsNull() const override {
			return true;
		}
		Type GetType() const override;
		VarPtr Ptr() const override { return ptrImpl<NullVar>(); }
		VarPtr CopyAsPtr() const override { return shallowCopy<NullVar>(); }
		bool Accept(Visitor& v, const char* name = nullptr) const override;
		const void* Data() const override {
			return nullptr;
		}
		bool Same(const VarInterface& right) const override;
		bool operator==(const VarInterface& right) const;
	};
}
