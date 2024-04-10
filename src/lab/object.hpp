#pragma once

#include "state.hpp"
#include "stack/include.hpp"
#include "function_wrapper.hpp"
#include "object_wrapper.hpp"
#include "registry.hpp"

namespace lab::object {

	template <typename T>
	struct instance {
		instance(lua_State* const state, const std::string_view name, const std::string_view table_name)
			: m_state(state)
			, m_name(name)
			, m_table(table_name)
		{
			object::names<T> = m_name;
			const auto L = m_state;

			stack::get_or_create_meta_table(L, m_name);

			stack::push<std::string_view>(L, "__index");
			stack::push_copy(L, -2);
			stack::set_table(L, -3);

			stack::push<std::string_view>(L, "__newindex");
			stack::push_copy(L, -2);
			stack::set_table(L, -3);

			const auto gc_fn = +[](lua_State* const L) -> std::int32_t {
				const auto obj = reinterpret_cast<object_wrapper::base*>(stack::get<void*>(L, -1));
				obj->~base();

				return 0;
			};

			stack::push<std::string_view>(L, "__gc");
			stack::push(L, gc_fn, 0);
			stack::set_table(L, -3);

			stack::clear(L);
		}

	public:
		template <typename... Args>
		void constructor()
		{
			this->constructor_impl<Args...>(std::index_sequence_for<Args...> {});
		}

		template <typename F> requires concepts::is_member_function<F>
		void function(const std::string_view name, const F fn)
		{
			const auto L = m_state;
			stack::get_or_create_meta_table(L, m_name);

			stack::push(L, name);
			stack::new_object<F>(L) = fn;

			stack::push(L, function_wrapper::get_struct(fn), 1);
			stack::set_table(L, -3);

			stack::pop(L);
		}

		// TODO: check at compile time that first argument to the function is a 'this' pointer
		template <typename F> requires concepts::is_function<F>
		void function(const std::string_view name, const F fn)
		{
			const auto L = m_state;
			stack::get_or_create_meta_table(L, m_name);

			stack::push(L, name);
			stack::push(L, reinterpret_cast<void*>(fn));

			stack::push(L, function_wrapper::get(fn), 1);
			stack::set_table(L, -3);

			stack::pop(L);
		}

	private:
		template <typename... Args, std::size_t... Is>
		void constructor_impl(const std::index_sequence<Is...>&&)
		{
			const auto new_fn = +[](lua_State* const L) -> std::int32_t {
				const auto name = stack::get<const char*>(L, lua_upvalueindex(1));
				stack::new_object<object_wrapper::ref<T>>(L, T { stack::get<Args>(L, Is + 1)... });

				stack::get_or_create_meta_table(L, name);
				stack::set_meta_table(L, -2);

				return 1;
			};

			const auto L = m_state;

			stack::get_or_create_table(L, m_table);
			stack::get_or_create_table(L, m_name);

			stack::push<std::string_view>(L, "new");

			stack::push(L, m_name);
			stack::push(L, new_fn, 1);

			stack::set_table(L, -3);
			stack::set_table(L, -3);

			stack::set_global(L, m_table);
		}

	private:
		lua_State* m_state = nullptr;

		std::string m_name = {};
		std::string m_table = {};
	};

}
