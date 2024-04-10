#pragma once
#include "include.hpp"

namespace lab::stack {

	template <typename T>
	inline auto new_user_data(lua_State* const L)
	{
		return reinterpret_cast<T*>(lua_newuserdata(L, sizeof(T)));
	}

	template <typename T, typename... Args>
	inline auto& new_object(lua_State* const L, Args&&... args)
	{
		const auto ret = new (stack::new_user_data<T>(L)) T { std::move(args)... };
		return *ret;
	}

}
