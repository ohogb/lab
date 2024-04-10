#pragma once
#include "include.hpp"

namespace lab::stack {

	inline void pop(lua_State* const L, const std::int32_t index = 1)
	{
		lua_pop(L, index);
	}

	inline void clear(lua_State* const L)
	{
		lua_settop(L, 0);
	}

	inline std::int32_t count(lua_State* const L)
	{
		return lua_gettop(L);
	}

}
