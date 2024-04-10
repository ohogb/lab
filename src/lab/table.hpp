#pragma once

#include "state.hpp"
#include "stack/include.hpp"
#include "function_wrapper.hpp"
#include "object.hpp"

namespace lab {

	struct table {
		table(lua_State* const state, const std::string_view table_name)
			: m_state(state)
			, m_table_name(table_name)
		{
		}

		template <typename T> requires lab::concepts::is_function<T>
		void function(const std::string_view name, const T fn)
		{
			const auto L = m_state;
			this->get();

			stack::push(L, name);

			stack::push(L, reinterpret_cast<void*>(fn));
			stack::push(L, function_wrapper::get(fn), 1);

			stack::set_table(L, -3);
			this->set();
		}

		template <typename T>
		void static_value(const std::string_view name, const T value)
		{
			const auto L = m_state;
			this->get();

			stack::push(L, name);
			stack::push(L, value);

			stack::set_table(L, -3);
			this->set();
		}

		template <typename T>
		auto new_object(const std::string_view name)
		{
			return std::make_unique<lab::object::instance<T>>(m_state, name, m_table_name);
		}

	private:
		void get()
		{
			const auto L = m_state;
			stack::get_or_create_table(L, m_table_name);
		}

		void set()
		{
			const auto L = m_state;
			stack::set_global(L, m_table_name);
		}

	private:
		lua_State* m_state = nullptr;
		std::string m_table_name = {};
	};

}
