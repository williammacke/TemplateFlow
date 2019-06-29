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

float var = .5;
float var2 = 0;

float p1 = 1.0f,p2=1.5f;


struct ZeroFunc {
	template <class T>
		int operator()(T t) {
			return 0;
		}

	template <class V>
		ZeroFunc getDerivative(V var) {
			return {};
		}
};

struct AddFive {
	template <class T>
		T operator()(T t) {
			return t+5;
		}

	template <class V>
		ZeroFunc getDerivative(V var) {
			return {};
		}
};

int main() {
	using namespace TF;
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

	Val<int, 1> a;
	FVal<Float<1,5>> c;
	auto d = a+c;
	constexpr auto e = d.getVal();

	std::cout << e << std::endl;

	Vector<Val<int, 5>, Val<int, 6>> vec;
	auto vec2 = map([](auto y) {return y+Val<int, 1>{}; }, vec);
	std::cout << head(vec2).getVal() << std::endl;

	constexpr auto forVal = For<1,10>(Val<int, 1>{}, [](auto i,auto y) {return y+i;}).getVal();
	std::cout << forVal << std::endl;

	constexpr auto WhileVal = While([](auto y) { return y.getVal() < 5; }, Val<int, 0>{}, [](auto y) {return y+Val<int, 1>{};}).getVal();
	std::cout << WhileVal << std::endl;

	constexpr ConstVector<Val<int, 5>, Val<int, 6>, Val<int, 7>> testVec;
	constexpr auto ForeachVal = Foreach(testVec, Val<int, 0>{}, [](auto i, auto y) { return i+y; }).getVal();
	std::cout << ForeachVal << std::endl;


	return 0;
}
