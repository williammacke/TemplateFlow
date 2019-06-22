#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "Operation.h"

template <class T, typename...Args>
struct GradientDescentOptimizerHelper;

template <class T, typename...Args>
struct GradientDescentOptimizer {
	constexpr static float learningRate = 0.1f;
	template <class E, class Iterator, class P, typename...Args2>
		static void Optimize(E expr, int numIterations, Iterator pFirst, Iterator pLast, P placeholder, Args2...args) {
			auto pFirst_cpy = pFirst;
			for (int i = 0; i < numIterations; i++) {
				pFirst = pFirst_cpy;
				while (pFirst != pLast) {
					P::setVal(*pFirst);
					GradientDescentOptimizerHelper<T, Args...>::Optimize(expr, learningRate, ++pFirst, pLast, args...);
				}
			}
		}
};

template <class T, typename...Args>
struct GradientDescentOptimizerHelper {
	template <class E, class Iterator>
		static void Optimize(E expr, float learningRate, Iterator& pFirst, Iterator& pLast) {
			T::addVal(-1*learningRate*E::getDerivative(T{}).getVal());
			GradientDescentOptimizerHelper<Args...>::Optimize(expr, learningRate, pFirst, pLast);
		}

	template <class E, class Iterator, class P, typename...Args2>
		static void Optimize(E expr, float learningRate, Iterator& pFirst, Iterator& pLast, P placeholder, Args2...args) {
			if (pFirst == pLast)
				return;
			P::setVal(*pFirst);
			Optimize(expr, learningRate, ++pFirst, pLast, args...);
		}
};

template <class T>
struct GradientDescentOptimizerHelper<T>{
	template <class E, class Iterator>
		static void Optimize(E expr, float learningRate, const Iterator& pFirst, const Iterator& pLast) {
			T::addVal(-1*learningRate*E::getDerivative(T{}).getVal());
		}
};


#endif
