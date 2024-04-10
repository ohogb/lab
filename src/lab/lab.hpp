#pragma once
#include <pch.hpp>

#include "instance.hpp"
#include "function.hpp"

namespace lab {

	inline auto create()
	{
		return std::make_unique<lab::instance>();
	}

}
