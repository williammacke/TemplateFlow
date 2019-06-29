#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include "Operation.h"
#include <cmath>
#include <tuple>
namespace TF {


	template <typename...Args>
		struct Vector {
			static std::tuple<decltype(Args::getVal())...>  getVal() {
				return std::make_tuple(Args::getVal()...);
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Vector<decltype(Args::getDerivative(var))...> {
					return {};
				}
			

		};

	template <typename...Args>
		struct ConstVector {
			static constexpr std::tuple<decltype(Args::getVal())...> getVal() {
				return std::make_tuple(Args::getVal()...);
			}

			template <class V>
				static constexpr auto getDerivative(V var) -> ConstVector<decltype(Args::getDerivative(var))...> {
					return {};
				}
		};

	template <typename...Args>
		struct isConstant<ConstVector<Args...>> {
			static constexpr bool value = true;
		};

	template <class T, typename...Args>
		constexpr Vector<T, Args...> vecConcat(Vector<T>, Vector<Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr Vector<T, Args...> vecConcat(ConstVector<T>, Vector<Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr Vector<T, Args...> vecConcat(Vector<T>, ConstVector<Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr ConstVector<T, Args...> vecConcat(ConstVector<T>, ConstVector<Args...>) {
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
		constexpr typename VarVector<T, data, l>::type makeVarVector() {
			return {};
		}

	template <class T, typename...Args>
		constexpr T head(Vector<T, Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr T head(ConstVector<T, Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr Vector<Args...> tail(Vector<T, Args...>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr ConstVector<Args...> tail(ConstVector<T, Args...>) {
			return {};
		}


	template <class T, class E>
		constexpr auto dot(T t, E e) {
			if constexpr(isConstant_v<E> && isConstant_v<T>) {
				return ConstAddition<ConstMultiplication<decltype(head(t)), decltype(head(e))>, decltype(dot(tail(t), tail(e)))>{};
			}
			else {
				return Addition<Multiplication<decltype(head(t)), decltype(head(e))>, decltype(dot(tail(t), tail(e)))>{};
			}
		}

	template <class T, class E>
		constexpr auto dot(Vector<T> t, Vector<E> e) -> Multiplication<T, E> {
			return {};
		}

	template <class T, class E>
		constexpr auto dot(ConstVector<T> t, ConstVector<E> e) -> Multiplication<T, E> {
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

	template <typename...Args>
		struct isVector<ConstVector<Args...>> {
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

	template <class T, typename...Args>
		struct isMatrix<ConstVector<T, Args...>> {
			constexpr static bool value = isVector<T>::value&&isMatrix<ConstVector<Args...>>::value;
		};

	template <class T>
		struct isMatrix<ConstVector<T>> {
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

	template <>
		struct isEmpty<ConstVector<>> {
			constexpr static bool value = true;
		};

	template <class T, typename...Args>
		struct isEmpty<Vector<T, Args...>> {
			constexpr static bool value = isEmpty<T>::value&&isEmpty<Vector<Args...>>::value;
		};

	template <class T, typename...Args>
		struct isEmpty<ConstVector<T, Args...>> {
			constexpr static bool value = isEmpty<T>::value&&isEmpty<ConstVector<Args...>>::value;
		};

	template <class T>
		constexpr bool isEmpty_v = isEmpty<T>::value;

	template <class T, class E>
		constexpr auto map(T t, E e) {
			if constexpr(isEmpty_v<E> && isConstant_v<E>) {
				return ConstVector<>{};
			}
			else if constexpr(isEmpty_v<E>) {
				return Vector<>{};
			}
			else if constexpr(isConstant_v<E>) {
				return vecConcat(ConstVector<decltype(t(head(e)))>{}, map(t, tail(e)));
			}
			else {
				return vecConcat(Vector<decltype(t(head(e)))>{}, map(t, tail(e)));
			}
		}

	struct Head {
		template <class T>
			constexpr auto operator()(T t) -> decltype(head(t)) {
				return {};
			}
	};

	struct Tail {
		template <class T>
			constexpr auto operator()(T t) -> decltype(tail(t)) {
				return {};
			}
	};

	template <class T>
		constexpr auto transpose(T t) {
			if constexpr(isEmpty_v<T>) {
				return Vector<>{};
			}
			return vecConcat(map(Head{}, t), transpose(map(Tail{}, t)));
		}

	template <class T, class E, typename=std::enable_if_t<isVector_v<T> && isVector_v<E>>>
		constexpr auto matMulHelper(T t, E e) {
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
		constexpr auto matMul(T t, E e) {
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

	template <class T>
	struct FuncApply;

	template <class T, class V> 
		constexpr auto funcApply(T t, V v) {
			if constexpr(!isVector_v<V>) {
				return func<T, V>{};
			}
			else {
				return map(FuncApply<T>{}, v);
			}
		}

	template <class T>
		struct FuncApply {
			template <class V>
			constexpr auto operator()(V v) {
				if constexpr(!isVector_v<V>) {
					return func<T, V>{};
				}
				else {
					return map(FuncApply<T>{}, v);
				}
			}
		};

}


#endif
