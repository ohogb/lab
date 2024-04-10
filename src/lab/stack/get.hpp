#pragma once
#include "include.hpp"

namespace lab {

	struct function;

}

namespace lab::stack {

	template <typename T>
	using rm = concepts::rm<T>;

	template <typename T> requires concepts::is_integer<T>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is<T>(L, index))
			return lua_tointeger(L, index);

		luaL_checktype(L, index, LUA_TNUMBER);
		return {};
	}

	template <typename T> requires concepts::is_float<T>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is<T>(L, index))
			return lua_tonumber(L, index);

		luaL_checktype(L, index, LUA_TNUMBER);
		return {};
	}

	template <typename T> requires concepts::is_boolean<T>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is<T>(L, index))
			return lua_toboolean(L, index);

		luaL_checktype(L, index, LUA_TBOOLEAN);
		return {};
	}

	template <typename T> requires concepts::is_string<T>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is<T>(L, index))
			return lua_tostring(L, index);

		if (stack::is<bool>(L, index))
			return stack::get<bool>(L, index) ? "true" : "false";

		luaL_checktype(L, index, LUA_TSTRING);
		return {};
	}

	template <typename T> requires((concepts::is_pointer<T> || concepts::is_function<T> || concepts::is_member_function<T>) && !concepts::is_struct<T>)
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is<T>(L, index))
			return reinterpret_cast<T>(lua_touserdata(L, index));

		luaL_checktype(L, index, LUA_TUSERDATA);
		return {};
	}

	template <typename T> requires(concepts::is_struct<T> && !std::is_same_v<rm<T>, lab::function>)
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (!stack::is<T>(L, index)) {
			luaL_checktype(L, index, LUA_TUSERDATA);
			return {};
		}

		const auto ptr = reinterpret_cast<object_wrapper::base*>(lua_touserdata(L, index));

		if constexpr (std::is_pointer_v<T>)
			return ptr->as<T>();

		else
			return *ptr->as<T*>();
	}

	template <typename T> requires std::is_same_v<rm<T>, lab::function>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (!stack::is_function(L, index)) {
			luaL_checktype(L, index, LUA_TFUNCTION);
			return {};
		}

		return T { L, index };
	}

	template <typename T> requires concepts::is_registry<T>
	inline rm<T> get(lua_State* const L, const std::int32_t index)
	{
		if (stack::is_nil(L, index)) {
			luaL_checktype(L, index, LUA_TUSERDATA);
			return {};
		}

		return T { L, index };
	}

}
