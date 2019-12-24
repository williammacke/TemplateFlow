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
#include "linalg.h"

void test(int i) { }

constexpr auto test2() {
	using namespace TF;
	Tensor<float, 5> b;
	b[1] = 1;
	return b;
}


int main() {
	using namespace TF;
	constexpr auto tester = test2();
	std::cout << tester[0] << std::endl;
	std::cout << tester[1] << std::endl;
	return 0;
}
