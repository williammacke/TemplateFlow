#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include "Operation.h"
#include <cmath>
#include <tuple>
namespace ML {


	template <typename...Args>
		struct Vector {
			static std::tuple<decltype(Args::getVal())...>  getVal() {
				return std::make_tuple(Args::getVal()...);
			}

			template <class V>
				static auto getDerivative(V var) -> Vector<decltype(Args::getDerivative(var))...> {
					return {};
				}
			

		};

	template <class T, typename...Args>
		Vector<T, Args...> vecConcat(Vector<T>, Vector<Args...>) {
			return {};
		}

	template <class T, T* data, int l>
		struct VarVector {
			using type = decltype(vecConcat(Vector<Var<T, data>>{}, typename VarVector<T, data+1, l-1>::type{}));
		};

	template <class T, T* data>
		struct VarVector<T, data, 1> {
			using type = Vector<Var<T, data>>;
		};

	template <class T, T* data, int l>
		typename VarVector<T, data, l>::type makeVarVector() {
			return {};
		}

	template <class T, typename...Args>
		T head(Vector<T, Args...>) {
			return {};
		}

	template <class T, typename...Args>
		Vector<Args...> tail(Vector<T, Args...>) {
			return {};
		}

	template <class T, class E>
		auto dot(T t, E e) ->Addition<Multiplication<decltype(head(t)), decltype(head(e))>, decltype(dot(tail(t), tail(e)))> {
			return {};
		}

	template <class T, class E>
		auto dot(Vector<T> t, Vector<E> e) -> Multiplication<T, E> {
			return {};
		}

	template <class T>
		struct isVector {
			constexpr static bool value = false;
		};

	template <typename...Args>
		struct isVector<Vector<Args...>> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool isVector_v = isVector<T>::value;

	template <class T>
		struct isMatrix {
			constexpr static bool value = false;
		};

	template <class T>
		constexpr bool isMatrix_v = isMatrix<T>::value;

	template <class T, typename...Args>
		struct isMatrix<Vector<T, Args...>> {
			constexpr static bool value = isVector<T>::value&&isMatrix<Vector<Args...>>::value;
		};

	template <class T>
		struct isMatrix<Vector<T>> {
			constexpr static bool value = isVector<T>::value;
		};

	template <class T, T *data, int r, int c>
		struct VarMatrix {
			using type = decltype(vecConcat(Vector<typename VarVector<T, data, c>::type>{}, typename VarMatrix<T, data+c, r-1, c>::type{}));
		};

	template <class T, T *data, int c>
		struct VarMatrix<T, data, 1, c> {
			using type = Vector<typename VarVector<T, data, c>::type>;
		};
	template <class T>
		struct isEmpty {
			constexpr static bool value = false;
		};

	template <>
		struct isEmpty<Vector<>> {
			constexpr static bool value = true;
		};

	template <class T, typename...Args>
		struct isEmpty<Vector<T, Args...>> {
			constexpr static bool value = isEmpty<T>::value&&isEmpty<Vector<Args...>>::value;
		};

	template <class T>
		constexpr bool isEmpty_v = isEmpty<T>::value;

	template <class T, class E>
		auto map(T t, E e) {
			if constexpr(isEmpty_v<E>) {
				return Vector<>{};
			}
			else {
				return vecConcat(Vector<decltype(t(head(e)))>{}, map(t, tail(e)));
			}
		}

	struct headFunc {
		template <class T>
			auto operator()(T t) -> decltype(head(t)) {
				return {};
			}
	};

	struct tailFunc {
		template <class T>
			auto operator()(T t) -> decltype(tail(t)) {
				return {};
			}
	};

	template <class T>
		auto transpose(T t) {
			if constexpr(isEmpty_v<T>) {
				return Vector<>{};
			}
			return vecConcat(map(headFunc{}, t), transpose(map(tailFunc{}, t)));
		}

	template <class T, class E, typename=std::enable_if_t<isVector_v<T> && isVector_v<E>>>
		auto matMulHelper(T t, E e) {
			if constexpr(isEmpty_v<T> || isEmpty_v<E>) {
				return Vector<>{};
			}
			else if constexpr(isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(matMulHelper(head(t), e), matMulHelper(tail(t), e));
			}
			else if constexpr(!isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(Vector<decltype(dot(t, head(e)))>{}, matMulHelper(t, tail(e)));
			}
			else if constexpr(isMatrix_v<T> && !isMatrix_v<E>) {
				return vecConcat(Vector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
			}
			else {
				return Vector<decltype(dot(t, e))>{};
			}
		}

	template <class T, class E, typename=std::enable_if_t<isVector_v<T> && isVector_v<E>>>
		auto matMul(T t, E e) {
			if constexpr(isEmpty_v<T> || isEmpty_v<E>) {
				return Vector<>{};
			}
			else if constexpr(isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(matMulHelper(head(t), transpose(e)), matMulHelper(tail(t), transpose(e)));
			}
			else if constexpr(!isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(Vector<decltype(dot(t, head(transpose(e))))>{}, matMulHelper(t, tail(transpose(e))));
			}
			else if constexpr(isMatrix_v<T> && !isMatrix_v<E>) {
				return vecConcat(Vector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
			}
			else {
				return Vector<decltype(dot(t, e))>{};
			}
		}

}


#endif
