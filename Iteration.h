#ifndef ITERATION_H_
#define ITERATION_H_
#include "Operation.h"
#include "linAlg_Operation.h"

namespace TF {
	template <int i, int j, class Init, class Op>
		constexpr auto For(Init init, Op op) {
			if constexpr(i == j) {
				return init;
			}
			else {
				if constexpr(i > j) {
					return For<i-1,j>(op(Val<int, i>{}, init), op);
				}
				else { 
					return For<i+1,j>(op(Val<int, i>{}, init), op);
				}
			}
		}

	template <int i, int j, int inc, class Init, class Op>
		constexpr auto For(Init init, Op op) {
			if constexpr(inc == 0) {
				return init;
			}
			if constexpr(inc > 0) {
				if constexpr(i >= j) {
					return init;
				}
				else {
					return For<i+inc, j, inc>(op(Val<int, i>{}, init), op);
				}
			}
			else {
				if constexpr(i <= j) {
					return init;
				}
				else {
					return For<i+inc, j , inc>(op(Val<int, i>{}, init), op);
				}
			}
		}

	template <class Cond, class Init, class Op>
		constexpr auto While(Cond cond, Init init, Op op) {
			if constexpr(!cond(init)) {
				return init;
			}
			else {
				return While(cond, op(init), op);
			}
		}

	template <class V, class Init, class Op>
		constexpr auto Foreach(V vec, Init init, Op op) {
			if constexpr(isEmpty_v<V>) {
				return init;
			}
			else {
				return Foreach(tail(vec), op(head(vec), init), op);
			}
		}
}

#endif
