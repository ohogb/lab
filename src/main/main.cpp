#include <pch.hpp>

#include <lab/lab.hpp>

struct Type {
	Type(const int32_t x)
	{
		fmt::println("Type::Type({})", x);
	}

	int32_t get_value(const int32_t x)
	{
		return x + 42;
	}
};

int main()
{
	static constexpr auto script = R"(
		print("hello from lua")

		local object = Type.new(123)
		local value = object:get_value(11)

		print("value: " .. value)
	)";

	const auto lab = lab::create();

	const auto global = lab->get_global_table();
	global->function(
		"print", +[](const std::string_view text) {
			fmt::println("{}", text);
		});

	const auto type = global->new_object<Type>("Type");
	type->constructor<int32_t>();
	type->function("get_value", &Type::get_value);

	if (const auto error = lab->execute_string(script)) {
		fmt::println("ERROR: {}", error);
		return 1;
	}
}
