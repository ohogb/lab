#pragma once

#include "state.hpp"
#include "stack/include.hpp"
#include "registry.hpp"

namespace lab {

	struct function {
		function() = default;

		function(lua_State* const state, const std::int32_t index)
			: m_reg(stack::get<lab::registry>(state, index))
		{
		}

		~function() = default;

		function(const function&) = delete;
		function& operator=(const function&) = delete;

		function(function&& o)
			: m_reg(std::move(o.m_reg))
		{
		}

		function& operator=(function&& o)
		{
			m_reg = std::move(o.m_reg);
			return *this;
		}

		// TODO: probably should never be returning a const or a reference
		template <typename T, typename... Args>
		T call(Args&&... args)
		{
			constexpr auto is_void = std::is_void_v<T>;

			const auto create_default = []() -> T {
				if constexpr (is_void)
					return;

				else
					return T {};
			};

			if (!m_reg.is_valid())
				return create_default();

			const auto L = m_reg.get_state();
			stack::push(L, m_reg);

			if (!lua_isfunction(L, -1)) {
				stack::pop(L);
				return create_default();
			}

			(stack::push(L, args), ...);

			if (lua_pcall(L, sizeof...(Args), (is_void ? 0 : 1), 0)) {
				// TODO: could cause a crash, probably should store a std::string and not a const char*
				m_error = stack::get<const char*>(L, -1);
				stack::clear(L);

				return create_default();
			}

			if constexpr (!is_void) {
				const auto ret = [L]() -> T {
					if (stack::is<T>(L, -1))
						return stack::get<T>(L, -1);

					return {};
				}();

				stack::clear(L);
				return ret;
			}

			stack::clear(L);
		}

		template <typename... Args>
		auto operator()(Args&&... args)
		{
			return this->call<void>(std::forward<Args>(args)...);
		}

		const char* get_error()
		{
			if (!m_error)
				return nullptr;

			return std::exchange(m_error, nullptr);
		}

	private:
		lab::registry m_reg = {};
		const char* m_error = nullptr;
	};

}
