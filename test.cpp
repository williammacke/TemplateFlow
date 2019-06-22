/** Sample program
 */
#include "Operation.h"
#include "Optimization.h"
#include "linAlg_Operation.h"
#include <iostream>
#include <vector>

int var = 7;
float var2 = 3.14;

int main() {
	ML::Val<int,5> a;
	ML::Var<int, &var> b;
	ML::Placeholder<int, &var> c;
	ML::FVal<ML::Float<314, -2>> d;
	std::cout << d.getVal() << std::endl;
	std::cout << ML::isOperation(b) << std::endl;
	auto test = a*b;
	std::cout << ML::isOperation(test) << " " << test.getDerivative(a).getVal() << std::endl;
	ML::Var<float, &var2> test2;
	auto test3 = test/test2;
	std::cout << test3.getVal() << std::endl;
	std::vector<float> vals = {3.14f, 3.22f, 1.0f};
	GradientDescentOptimizer<decltype(b), decltype(test2)>::Optimize(test3, 5, vals.begin(), vals.end(), c);
	std::cout << test3.getVal() << std::endl;
	std::cout << var << std::endl;
	std::cout << var2 << std::endl;
	using namespace ML;
	ML::MatrixFVal<2, 2, Float<2,1>, Float<3,1>, Float<4,1>, Float<5,1>> mat;
	for (int i = 0; i < 4; i++) {
		std::cout << mat.getVal()[i] << std::endl;
	}
	return 0;
}
