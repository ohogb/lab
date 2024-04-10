#pragma once

#include "state.hpp"
#include "table.hpp"
#include "stack/include.hpp"

namespace lab {

	struct instance {
		instance() { this->create(); }
		~instance() { this->release(); }

		void create()
		{
			auto& L = m_state;

			if (L)
				return;

			L = luaL_newstate();
			luaL_openlibs(L);

			lab::state::instances.push_back(L);
		}

		void release()
		{
			auto& L = m_state;

			if (!L)
				return;

			if (const auto it = std::ranges::find(lab::state::instances, L); it != lab::state::instances.end())
				lab::state::instances.erase(it);

			lua_close(L);
			L = nullptr;
		}

		void reset()
		{
			this->release();
			this->create();
		}

		const char* execute_string(const std::string_view script)
		{
			const auto L = m_state;

			if (!luaL_dostring(L, script.data()))
				return nullptr;

			const auto error = stack::get<const char*>(L, -1);
			stack::pop(L);

			return error;
		}

		const char* execute_file(const std::string_view file_name)
		{
			const auto L = m_state;

			if (!luaL_dofile(L, file_name.data()))
				return nullptr;

			const auto error = stack::get<const char*>(L, -1);
			stack::pop(L);

			return error;
		}

		auto get_global_table()
		{
			return std::make_unique<lab::table>(m_state, "_G");
		}

		auto get_table(const std::string_view name)
		{
			return std::make_unique<lab::table>(m_state, name);
		}

	private:
		lua_State* m_state = nullptr;
	};

}
