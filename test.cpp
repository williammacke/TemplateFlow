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

	std::cout << vecLength(vec) << std::endl;
	

	constexpr auto blargh1 = makeValMatrix<int, 2, 2, 1, 2, 3, 4>();
	constexpr auto blargh2 = makeValVector<int, 5, 6>();
	constexpr auto blargh3 = matMul(blargh1, blargh2);
	constexpr auto blargh4 = blargh3.getVal();
	std::cout << blargh4[0] << std::endl;

	constexpr auto primesInit = map([](auto i) {
			if constexpr(i.getVal() == 0 || i.getVal() == 1) {
				return Val<bool, false>{};
			}
			else {
				return Val<bool, true>{};
			}
			},
			range<0,200>());

	constexpr auto primes = For<0, 200>(primesInit, [](auto val, auto primes) {
				if constexpr(!get<val.getVal()>(primes).getVal()) {
					return primes;
				}
				else {
					return For<val.getVal()+val.getVal(), 200, val.getVal()>(primes, [](auto i, auto p) {
								return set<i.getVal()>(p, Val<bool, false>{});
							});
				}
			}).getVal();

	for (int i = 0; i < primes.length; i++) {
		if (primes[i]) {
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;



	auto testVec1 = makeValVector<int, 1,2,3>();
	auto testVec2 = makeValVector<int, 4,5,6>();
	auto testVec3 = vecConcat(testVec1, testVec2);



	return 0;
}
