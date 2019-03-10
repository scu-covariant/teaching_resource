#include "./any.hpp"
#include <typeindex>
#include <iostream>
#include <string>
template <std::size_t N>
class value_holder<char[N]> : public value_holder<std::string> {
public:
	using value_holder<std::string>::value_holder;
};
template <>
class value_holder<std::type_info> : public value_holder<std::type_index> {
public:
	using value_holder<std::type_index>::value_holder;
};
class foo final {
public:
	foo()
	{
		std::cerr << "Foo normal construct" << std::endl;
	}
	foo(const foo &)
	{
		std::cerr << "Foo copy construct" << std::endl;
	}
	~foo()
	{
		std::cerr << "Foo destroy" << std::endl;
	}
};
int main()
{
	any a = 10, b = "Hello", c = foo();
	const any d = b.type();
	a = 3.14;
	std::string tmp = b;
	b = tmp + ", World!";
	std::cout << a.value<double>() << std::endl;
	a = b;
	std::cout << a.const_value<std::string>() << std::endl;
	a = c;
	std::cout << d.value<std::type_index>().name() << std::endl;
	return 0;
}