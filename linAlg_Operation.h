#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include "Operation.h"
#include <cmath>
#include <tuple>
namespace ML {


	template <typename...Args>
		struct Vector {
			static std::tuple<Args...>  getVal() {
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
		struct VarVector<T, data, 0> {
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
}


#endif
