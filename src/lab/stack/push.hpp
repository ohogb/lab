#pragma once
#include "include.hpp"

#include "../state.hpp"
#include "../object_wrapper.hpp"

namespace lab::stack {

	inline std::int32_t push_nil(lua_State* const L)
	{
		lua_pushnil(L);
		return 1;
	}

	inline std::int32_t push_copy(lua_State* const L, const std::int32_t index)
	{
		lua_pushvalue(L, index);
		return 1;
	}

	template <typename T> requires concepts::is_integer<T>
	inline std::int32_t push(lua_State* const L, const T val)
	{
		lua_pushinteger(L, val);
		return 1;
	}

	template <typename T> requires concepts::is_float<T>
	inline std::int32_t push(lua_State* const L, const T val)
	{
		lua_pushnumber(L, val);
		return 1;
	}

	template <typename T> requires concepts::is_boolean<T>
	inline std::int32_t push(lua_State* const L, const T val)
	{
		lua_pushboolean(L, val);
		return 1;
	}

	template <typename T> requires concepts::is_string<T>
	inline std::int32_t push(lua_State* const L, const T& val)
	{
		if constexpr (std::is_same_v<T, std::string>)
			lua_pushstring(L, val.c_str());

		else if constexpr (std::is_same_v<T, std::string_view>)
			lua_pushstring(L, val.data());

		else
			lua_pushstring(L, val);

		return 1;
	}

	template <typename T> requires(concepts::is_pointer<T> && !concepts::is_struct<T>)
	inline std::int32_t push(lua_State* const L, const T val)
	{
		if (val == nullptr)
			stack::push_nil(L);

		else
			lua_pushlightuserdata(L, val);

		return 1;
	}

	template <typename T> requires concepts::is_struct<T>
	inline std::int32_t push(lua_State* const L, T val)
	{
		const auto& name = lab::object::names<std::remove_pointer_t<std::decay_t<T>>>;

		if (name.empty()) {
			luaL_error(L, "metatable for this structure doesn't exist!");
			return 0;
		}

		if constexpr (std::is_pointer_v<T>) {
			if (val == nullptr)
				stack::push_nil(L);

			else
				stack::new_object<object_wrapper::pointer>(L, val);
		}

		else if constexpr (std::is_lvalue_reference_v<T>)
			stack::new_object<object_wrapper::pointer>(L, &val);

		else
			stack::new_object<object_wrapper::ref<T>>(L, std::move(val));

		luaL_getmetatable(L, name.c_str());
		lua_setmetatable(L, -2);

		return 1;
	}

	template <typename T> requires concepts::is_lua_c_function<T>
	inline std::int32_t push(lua_State* const L, const T val, const std::int32_t up_values)
	{
		lua_pushcclosure(L, val, up_values);
		return 1;
	}

	template <typename T> requires concepts::is_registry<T>
	inline std::int32_t push(lua_State* const L, const T& val)
	{
		if (!val.is_valid())
			return 0;

		const auto index = val.get_index();
		lua_rawgeti(L, LUA_REGISTRYINDEX, index);

		return 1;
	}

}
