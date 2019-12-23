/** Sample program
 */
#include "Operation.h"
#include "Optimization.h"
#include "lists.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "math.h"
#include <cmath>
#include "Iteration.h"

void test(int i) { }


int main() {
	using namespace TF;
	constexpr auto a = Variable("test"_tstr);
	std::cout << a.getVal() << std::endl;
	constexpr auto b = Value<int>(5);
	constexpr auto c = b+5;
	std::cout << (int) c << std::endl;
	constexpr auto d = b+c;
	std::cout << (int) d << std::endl;
	constexpr auto e = a + d;
	std::cout << (int) e << std::endl;
	std::cout << a.getName() << std::endl;
	a = 5;
	std::cout << (int) e << std::endl;
	test(a);
	a = 5;
	return 0;
}
