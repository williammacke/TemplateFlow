#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "Operation.h"
#include "linAlg_Operation.h"
#include <iostream>

namespace TF{

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
							GradientDescentOptimizerHelper<T, Args...>::Optimize(expr, learningRate, pFirst, pLast, placeholder, args...);
						}
					}
				}
		};


	template <class T, typename...Args>
		struct GradientDescentOptimizerHelper {

			template <class V1, class V2>
				static void adjustVec(V1 v1, V2 v2, float learningRate) {
					if constexpr(isEmpty_v<V1>) {
						return;
					}
					else if constexpr(isVector_v<decltype(head(v1))>) {
						adjustVec(head(v1), head(v2), learningRate);
						adjustVec(tail(v1), tail(v2), learningRate);
					}
					else {
						head(v1).addVal(-1*learningRate*head(v2).getVal());
						adjustVec(tail(v1), tail(v2), learningRate);
					}
				}

			template <class E, class Iterator>
				static void Optimize(E expr, float learningRate, Iterator& pFirst, Iterator& pLast) {
					if constexpr(isVector_v<T>) {
						constexpr auto grad = gradient(expr, T{});
						adjustVec(T{}, grad, learningRate);

					}
					else {
						T::addVal(-1*learningRate*E::getDerivative(T{}).getVal());
					}
					GradientDescentOptimizerHelper<Args...>::Optimize(expr, learningRate, pFirst, pLast);
				}

			template <class E, class Iterator, class P, typename...Args2>
				static void Optimize(E expr, float learningRate, Iterator& pFirst, Iterator& pLast, P placeholder, Args2...args) {
					if (pFirst == pLast)
						return;
					if constexpr(isVector_v<P>) {
						if constexpr(isEmpty_v<P>) {
							Optimize(expr, learningRate, pFirst, pLast, args...);
						}
						else {
							Optimize(expr, learningRate, pFirst, pLast, head(placeholder), tail(placeholder), args...);
						}
					}
					else {
						P::setVal(*pFirst);
						Optimize(expr, learningRate, ++pFirst, pLast, args...);
					}
				}

		};

	template <class T>
		struct GradientDescentOptimizerHelper<T>{
			template <class V1, class V2>
				static void adjustVec(V1 v1, V2 v2, float learningRate) {
					if constexpr(isVector_v<decltype(head(v1))>) {
						adjustVec(head(v1), head(v2), learningRate);
						adjustVec(tail(v1), tail(v2), learningRate);
					}
					else {
						head(v1).addVal(-1*learningRate*head(v2).getVal());
						adjustVec(tail(v1), tail(v2), learningRate);
					}
				}

			template <class E, class Iterator>
				static void Optimize(E expr, float learningRate, const Iterator& pFirst, const Iterator& pLast) {
					if constexpr(isVector_v<T>) {
						auto grad = gradient(expr, T{});
						adjustVec(T{}, grad, learningRate);

					}
					else {
						T::addVal(-1*learningRate*E::getDerivative(T{}).getVal());
					}
				}
		};
}


#endif
