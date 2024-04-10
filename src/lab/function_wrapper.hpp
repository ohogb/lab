#pragma once

#include "state.hpp"
#include "stack/include.hpp"

namespace lab::function_wrapper {

	template <typename T, typename... Args, std::size_t... Is>
	inline auto impl(T (*const fn)(Args...), const std::index_sequence<Is...>&&)
	{
		const auto ret = +[](lua_State* const L) -> std::int32_t {
			const auto function = stack::get<decltype(fn)>(L, lua_upvalueindex(1));

			if constexpr (std::is_void_v<T>) {
				function(stack::get<Args>(L, Is + 1)...);
				stack::clear(L);

				return 0;
			}

			else {
				decltype(auto) ret = function(stack::get<Args>(L, Is + 1)...);
				stack::clear(L);

				if constexpr (std::is_lvalue_reference_v<decltype(ret)>)
					return stack::push<decltype(ret)>(L, ret);

				else
					return stack::push(L, std::move(ret));
			}
		};

		return ret;
	}

	template <typename T, typename TS, typename... Args, std::size_t... Is>
	inline auto impl_struct(T (TS::*const fn)(Args...), const std::index_sequence<Is...>&&)
	{
		const auto ret = +[](lua_State* const L) -> std::int32_t {
			const auto function = *stack::get<decltype(fn)*>(L, lua_upvalueindex(1));
			const auto this_address = stack::get<TS*>(L, 1);

			if constexpr (std::is_void_v<T>) {
				(this_address->*function)(stack::get<Args>(L, Is + 2)...);
				stack::clear(L);

				return 0;
			}

			else {
				decltype(auto) ret = (this_address->*function)(stack::get<Args>(L, Is + 2)...);
				stack::clear(L);

				if constexpr (std::is_lvalue_reference_v<decltype(ret)>)
					return stack::push<decltype(ret)>(L, ret);

				else
					return stack::push(L, std::move(ret));
			}
		};

		return ret;
	}

	template <typename T, typename... Args>
	inline auto get(T (*const fn)(Args...))
	{
		const auto ret = function_wrapper::impl(fn, std::index_sequence_for<Args...> {});
		return ret;
	}

	template <typename T, typename TS, typename... Args>
	inline auto get_struct(T (TS::*const fn)(Args...))
	{
		const auto ret = function_wrapper::impl_struct(fn, std::index_sequence_for<Args...> {});
		return ret;
	}

	// for const member functions
	template <typename T, typename TS, typename... Args>
	inline auto get_struct(T (TS::*const fn)(Args...) const)
	{
		const auto ret = function_wrapper::impl_struct(reinterpret_cast<T (TS::*)(Args...)>(fn), std::index_sequence_for<Args...> {});
		return ret;
	}

}
