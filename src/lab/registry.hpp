#pragma once

#include "state.hpp"
#include "stack/include.hpp"

namespace lab {

	struct registry {
		registry() = default;

		registry(lua_State* const state, const std::int32_t index)
			: m_state(state)
		{
			const auto L = m_state;

			stack::push_copy(L, index);
			m_id = luaL_ref(L, LUA_REGISTRYINDEX);
		}

		~registry()
		{
			if (!this->is_valid())
				return;

			this->unref();
		}

		// for now
		registry(const registry&) = delete;
		registry& operator=(const registry&) = delete;

		registry(registry&& o)
			: m_state(std::exchange(o.m_state, nullptr))
			, m_id(std::exchange(o.m_id, LUA_NOREF))
		{
		}

		registry& operator=(registry&& o)
		{
			m_state = std::exchange(o.m_state, nullptr);
			m_id = std::exchange(o.m_id, LUA_NOREF);
			return *this;
		}

		auto get_state() const
		{
			return m_state;
		}

		auto get_index() const
		{
			return m_id;
		}

		bool is_valid() const
		{
			const auto L = this->get_state();

			if (L == nullptr)
				return false;

			if (m_id == LUA_NOREF)
				return false;

			// reset?
			if (std::ranges::find(lab::state::instances, L) == lab::state::instances.end())
				return false;

			return true;
		}

	private:
		void unref()
		{
			if (this->is_valid())
				luaL_unref(this->get_state(), LUA_REGISTRYINDEX, m_id);

			m_state = nullptr;
			m_id = LUA_NOREF;
		}

	private:
		lua_State* m_state = nullptr;
		std::int32_t m_id = LUA_NOREF;
	};

}
