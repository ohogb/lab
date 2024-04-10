#pragma once
#include "include.hpp"

namespace lab::stack {

	template <typename T> requires concepts::is_integer<T>
	inline bool is(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isnumber(L, index);
		return ret;
	}

	template <typename T> requires concepts::is_float<T>
	inline bool is(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isnumber(L, index);
		return ret;
	}

	template <typename T> requires concepts::is_boolean<T>
	inline bool is(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isboolean(L, index);
		return ret;
	}

	template <typename T> requires concepts::is_string<T>
	inline bool is(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isstring(L, index);
		return ret;
	}

	template <typename T> requires concepts::is_pointer<T> || concepts::is_struct<T> || concepts::is_function<T>
	inline bool is(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isuserdata(L, index);
		return ret;
	}

	inline bool is_function(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isfunction(L, index);
		return ret;
	}

	inline bool is_table(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_istable(L, index);
		return ret;
	}

	inline bool is_nil(lua_State* const L, const std::int32_t index)
	{
		const auto ret = lua_isnil(L, index);
		return ret;
	}

}
