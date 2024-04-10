#pragma once

#include <pch.hpp>
#include "../object_wrapper.hpp"

namespace lab {

	struct registry;

}

namespace lab::concepts {

	template <typename T>
	using rm = std::decay_t<T>;

	template <typename T>
	concept is_boolean = std::is_same_v<rm<T>, bool>;

	template <typename T>
	concept is_integer = std::is_integral_v<rm<T>> && !concepts::is_boolean<T>;

	template <typename T>
	concept is_float = std::is_floating_point_v<rm<T>> && !concepts::is_integer<T>;

	template <typename T>
	concept is_string = std::is_same_v<rm<T>, std::string_view> || std::is_same_v<rm<T>, std::string> || std::is_same_v<rm<T>, const char*>;

	template <typename T>
	concept is_function = std::is_function_v<std::remove_pointer_t<T>>;

	template <typename T>
	concept is_lua_c_function = std::is_same_v<T, lua_CFunction>;

	template <typename T>
	concept is_member_function = std::is_member_function_pointer_v<T>;

	template <typename T>
	concept is_pointer = std::is_pointer_v<T> && !concepts::is_string<T> && !concepts::is_function<T> && !concepts::is_member_function<T>;

	template <typename T>
	concept is_registry = std::is_same_v<rm<T>, lab::registry>;

	template <typename T>
	concept is_struct = std::is_class_v<std::remove_pointer_t<rm<T>>> && !concepts::is_function<T> && !concepts::is_string<T> && !concepts::is_registry<T>;

}
