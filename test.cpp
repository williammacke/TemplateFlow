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
#include "lists.h"

void test(int i) { }

constexpr auto test2() {
	using namespace TF;
	Tensor<float, 5> b;
	b[1] = 1;
	return b;
}


int main() {
	using namespace TF;
	constexpr auto a = test2();
	constexpr Tensor<float, 2> b = {1,2};
	constexpr Tensor<float, 2, 2> c = { {1,2}, {3,4}};
	return 0;
}
