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
		Type GetType() const override;
		operator bool() const override;
		std::string ToStr() const override;
		const void* Data() const override;
		vl::VarPtr Copy() const override;
		const VarPtr& operator[](const char* s) const;
		VarPtr& operator[](const char* s);
		bool Same(const VarInterface& other) const override;

	protected:
		const utils::entity* self() const override;
	};
}
