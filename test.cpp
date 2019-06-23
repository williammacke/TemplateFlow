/** Sample program
 */
#include "Operation.h"
#include "Optimization.h"
#include <iostream>
#include <vector>

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
	auto err = ((y-line)^Val<int, 2>{});
	std::vector<float> data = {1, 1.4f,
				2, 2.6f,
				3,3.5f,
				4,4.4f,
				5, 5.55f};
	std::cout << err.getDerivative(m).getVal() << std::endl;
					
	GradientDescentOptimizer<decltype(m), decltype(b)>::Optimize(err, 500, data.begin(), data.end(), x, y);
	std::cout << var << " " << var2 << std::endl;
	return 0;
}
