#pragma once
#include <pch.hpp>

namespace lab::state {

	inline auto instances = std::deque<lua_State*> {};

}

namespace lab::object {

	template <typename T>
	inline auto names = std::string {};

}
