#pragma once
#include "include.hpp"

namespace lab::stack {

	inline void set_table(lua_State* const L, const std::int32_t index)
	{
		lua_settable(L, index);
	}

	inline void set_meta_table(lua_State* const L, const std::int32_t index)
	{
		lua_setmetatable(L, index);
	}

	inline void set_global(lua_State* const L, const std::string_view name)
	{
		lua_setglobal(L, name.data());
	}

	inline void get_or_create_table(lua_State* const L, const std::string_view name)
	{
		lua_getglobal(L, name.data());

		if (!stack::is_nil(L, -1))
			return;

		stack::pop(L);
		stack::push(L, name);

		lua_newtable(L);
	}

	inline void get_or_create_meta_table(lua_State* const L, const std::string_view name)
	{
		luaL_getmetatable(L, name.data());

		if (!stack::is_nil(L, -1))
			return;

		stack::pop(L);
		luaL_newmetatable(L, name.data());
	}

}
