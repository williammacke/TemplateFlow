/** Sample program
 */
#include "Operation.h"
#include "Optimization.h"
#include "linAlg_Operation.h"
#include <iostream>
#include <vector>
#include <tuple>

float var = .5;
float var2 = 0;

float p1 = 1.0f,p2=1.5f;

int main() {
	using namespace ML;
	Var<float, &var> m;
	Var<float, &var2> b;
	Placeholder<float, &p1> x;
	Placeholder<float, &p2> y;
	auto line = m*x+b;
	auto err = ((y-line)^Val<int, 2>{})/Val<int, 2>{};
	std::vector<float> data = {1, 1.5f,
				2, 2.5f,
				3,3.5f,
				4,4.5f,
				5, 5.5f};
	std::cout << err.getDerivative(m).getVal() << std::endl;
					
	GradientDescentOptimizer<decltype(m), decltype(b)>::Optimize(err, 500, data.begin(), data.end(), x, y);
	std::cout << var << " " << var2 << std::endl;

	Vector<Vector<Val<int, 1>, Val<int, 2>>, Vector<Val<int, 3>, Val<int,4>>> a;
	Vector<Val<int, 1>, Val<int, 1>> d;
	auto c = matMul(a, d);
	std::cout << std::get<0>(c.getVal()) << std::endl;
	return 0;
}
