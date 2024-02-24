#pragma once
#include <memory>
#include <stddef.h>
#include <vl/var_interface.h>

namespace vl
{
	class VarPtr : public VarInterface
	{
		using Ptr_t = std::shared_ptr<Var>;
		Ptr_t mPtr;

	public:
		VarPtr() = default;
		VarPtr(const Ptr_t& ptr) : mPtr(ptr) {}
		VarPtr(std::nullptr_t null_ptr) : mPtr(nullptr) {}
		VarPtr(const char* str);
		VarPtr(const std::string& str);
		VarPtr(bool b);
		VarPtr(int i);
		VarPtr(const void* ptr);
		bool operator==(const VarPtr& right);
		bool operator==(std::nullptr_t null_ptr) {
			return mPtr == nullptr;
		}
		bool operator!=(const VarPtr& right) {
			return !(*this == right);
		}
		bool operator!=(std::nullptr_t null_ptr) {
			return mPtr != null_ptr;
		}
		template <typename T>
		VarPtr& operator=(const T& value) {
			*this = MakePtr(value);
			return *this;
		}
		Var* operator->() {
			return mPtr.get();
		}
		const Var* operator->() const {
			return mPtr.get();
		}
		Var& operator*() {
			return *mPtr;
		}
		const Var& operator*() const {
			return *mPtr;
		}
		const BoolVar& AsBool() const override;
		const NumberVar& AsNumber() const override;
		const StringVar& AsString() const override;
		const PointerVar& AsPointer() const override;
		const ObjectVar& AsObject() const override;
		const ListVar& AsList() const override;
		BoolVar& AsBool() override;
		NumberVar& AsNumber() override;
		StringVar& AsString() override;
		PointerVar& AsPointer() override;
		ObjectVar& AsObject() override;
		ListVar& AsList() override;
		bool IsBool() const override;
		bool IsNumber() const override;
		bool IsString() const override;
		bool IsPointer() const override;
		bool IsObject() const override;
		bool IsList() const override;
		bool IsNull() const override;
		Type GetType() const override;
		operator bool() const override;
		std::string ToStr() const override;
		const void* Data() const override;
		vl::VarPtr Copy() const override;
		const VarPtr& operator[](const char* s) const override;
		VarPtr& operator[](const char* s) override;
	};
}
