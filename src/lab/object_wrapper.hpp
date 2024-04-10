#pragma once
#include <pch.hpp>

namespace lab::object_wrapper {

	struct base {
		virtual ~base() = default;
		virtual void* get() = 0;

		template <typename T>
		T as()
		{
			return reinterpret_cast<T>(this->get());
		}
	};

	struct pointer : public base {
		pointer(void* const addr)
			: m_address(addr) {};

		~pointer()
		{
			m_address = nullptr;
		}

		virtual void* get() override
		{
			return m_address;
		}

	private:
		void* m_address = nullptr;
	};

	template <typename T>
	struct ref : public base {
		ref(const T& val)
			: m_value(val)
		{
		}

		ref(T&& val)
			: m_value(std::move(val))
		{
		}

		virtual void* get() override
		{
			return &m_value;
		}

	private:
		T m_value = {};
	};

}
