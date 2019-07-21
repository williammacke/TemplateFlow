/** Sample program
 */
#include "Operation.h"
#include "Optimization.h"
#include "linAlg_Operation.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "math.h"
#include <cmath>
#include "Iteration.h"


int main() {
	using namespace TF;

	constexpr auto y = Val<int, 5>{} / Val<long, 10>{};
	constexpr auto val = y.getVal();

	constexpr auto y2 = ln(Val<int, 3>{});
	constexpr auto val2 = y2.getVal();

	std::cout << val << std::endl;
	std::cout << val2 << std::endl;

	constexpr auto fib = For<1, 40>(makeVector(Val<long, 0>{}, Val<long, 1>{}), [](auto in, auto val) {
			constexpr auto i = in.getVal();
			return val+makeVector(get<i>(val)+get<i-1>(val));
			}).getVal();

	for (int i = 0; i < fib.length; i++) {
		std::cout << fib[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}
