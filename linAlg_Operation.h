#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include "Operation.h"
#include <cmath>
#include <tuple>
#include <initializer_list>
namespace TF {

	template <class V>
		constexpr bool isSame(V v);

	template <class V>
		struct isEmpty;

	template <class T, size_t size>
		struct Array {

			constexpr Array(std::initializer_list<T> d) : data() {
				for (int i = 0; i < size; i++) {
					data[i] = *(d.begin()+i);
				}
			}

			T operator[](unsigned int i) const {
				return data[i];
			}

			constexpr static size_t length = size;
			private:
				T data[size];
		};

	template <typename...Args>
		struct Vector {
			static auto  getVal() {
				if constexpr(isSame(Vector<Args...>{}) && !isEmpty<Vector<Args...>>::value) {
					return Array<decltype(head(Vector<Args...>{}).getVal()), vecLength(Vector<Args...>{})>{Args::getVal()...};
				}
				else {
					return std::make_tuple(Args::getVal()...);
				}
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Vector<decltype(Args::getDerivative(var))...> {
					return {};
				}



		};

	template <typename...Args>
		struct ConstVector {
			static constexpr auto getVal() {
				if constexpr(isSame(ConstVector<Args...>{}) && !isEmpty<ConstVector<Args...>>::value) {
					return Array<decltype(head(ConstVector<Args...>{}).getVal()), vecLength(ConstVector<Args...>{})>{Args::getVal()...};
				}
				else {
					return std::make_tuple(Args::getVal()...);
				}
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


	constexpr auto makeVector() {
		return ConstVector<>{};
	}

	template <class T>
		constexpr auto makeVector(T t) {
			if constexpr(isConstant_v<T>) {
				return ConstVector<T>{};
			}
			else {
				return Vector<T>{};
			}

		}

	template <class T, typename...Args>
		constexpr auto makeVector(T t, Args...args) {
			return vecConcat(makeVector(t), makeVector(args...));  
		}

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

	template <class T, T...vals>
		struct ValVector {
			using type = ConstVector<Val<T, vals>...>;
		};

	template <class T, T...vals>
		constexpr typename ValVector<T,vals...>::type makeValVector() {
			return {};
		}

	template <typename...Args>
		struct FValVector {
			using type = ConstVector<FVal<Args>...>;
		};

	template <typename...Args>
		constexpr typename FValVector<Args...>::type makeFValVector() {
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

	template <class T, typename...Args>
		constexpr T end(Vector<T>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr auto end(Vector<T, Args...> v) {
			return end(tail(v));
		}

	template <class T, typename...Args>
		constexpr T end(ConstVector<T>) {
			return {};
		}

	template <class T, typename...Args>
		constexpr auto end(ConstVector<T, Args...> v) -> decltype(end(v)) {
			return end(tail(v));
		}

	template <class V>
		constexpr size_t vecLength(V v) {
			return 1+vecLength(tail(v));
		}

	constexpr size_t vecLength(Vector<>) {
		return 0;
	}

	constexpr size_t vecLength(ConstVector<>) {
		return 0;
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

	template <unsigned int n, class V>
		constexpr auto get(V v) {
			if constexpr(n == 0) {
				return head(v);
			}
			else {
				return get<n-1>(tail(v));
			}

		}

	template <class V>
		constexpr auto empty(V v) {
			if constexpr(isConstant_v<V>) {
				return ConstVector<>{};
			}
			else {
				return Vector<>{};
			}
		}

	template <unsigned int n, class V>
		constexpr auto first(V v) {
			if constexpr(n == 0) {
				return empty(v);
			}
			else if constexpr(n == 1) {
				if constexpr(isConstant_v<V>) {
					return ConstVector<decltype(head(v))>{};
				}
				else {
					return Vector<decltype(head(v))>{};
				}
			}
			else {
				if constexpr(isConstant_v<V>) {
					return vecConcat(ConstVector<decltype(head(v))>{}, first<n-1>(tail(v)));
				}
				else {
					return vecConcat(Vector<decltype(head(v))>{}, first<n-1>(tail(v)));
				}
			}

		}

	template <int n, class V>
		constexpr auto after(V v) {
			if constexpr(n <= 0) {
				return v;
			}
			else {
				return after<n-1>(tail(v));
			}
		}

	template <unsigned int n, class V>
		constexpr auto last(V v) {
			constexpr auto size = vecLength(v);
			return after<size-n>(v);
		}




	template <class T, class E>
		constexpr auto dot(Vector<T> t, Vector<E> e) -> Multiplication<T, E> {
			return {};
		}

	template <class T, class E>
		constexpr auto dot(ConstVector<T> t, ConstVector<E> e) -> ConstMultiplication<T, E> {
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

	template <class T, int r, int c,  T *data>
		constexpr typename VarMatrix<T, data, r, c>::type makeVarMatrix() {
			return {};
		}

	template <int r, int c, class V>
		struct ValMatrixHelper {
			using type = decltype(vecConcat(ConstVector<decltype(first<c>(V{}))>{}, typename ValMatrixHelper<r-1, c, decltype(after<c>(V{}))>::type{}));
		};

	template <int c, class V>
		struct ValMatrixHelper<1, c, V> {
			static_assert(vecLength(V{})==c, "Matrix Dimensions do not match");
			using type = ConstVector<V>;
		};

	template <class T, int r, int c, T...vals>
		struct ValMatrix {
			using V = ConstVector<Val<T, vals>...>;
			using type = decltype(vecConcat(ConstVector<decltype(first<c>(V{}))>{}, typename ValMatrixHelper<r-1, c, decltype(after<c>(V{}))>::type{}));
		};

	template <class T, int r, int c, T...vals>
		constexpr typename ValMatrix<T, r, c, vals...>::type makeValMatrix() {
			return {};
		}

	template <int r, int c, typename...vals>
		struct FValMatrix {
			using V = ConstVector<vals...>;
			using type = decltype(vecConcat(ConstVector<decltype(first<c>(V{}))>{}, typename ValMatrixHelper<r-1, c, decltype(after<c>(V{}))>::type{}));
		};

	template <int r, int c, typename...vals>
		constexpr typename FValMatrix<r, c, vals...>::type makeFValMatrix() {
			return {};
		}


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

	template <class V>
		constexpr bool isSame(V v) {
			if constexpr(vecLength(v) <= 1) {
				return true;
			}
			else {
				return isSame(get<0>(v), get<1>(v)) && isSame(tail(v));
			}
		}

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
				return ConstVector<>{};
			}
			else if constexpr(isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(matMulHelper(head(t), e), matMulHelper(tail(t), e));
			}
			else if constexpr(!isMatrix_v<T> && isMatrix_v<E>) {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return vecConcat(ConstVector<decltype(dot(t, head(e)))>{}, matMulHelper(t, tail(e)));
				}
				else {
					return vecConcat(Vector<decltype(dot(t, head(e)))>{}, matMulHelper(t, tail(e)));
				}
			}
			else if constexpr(isMatrix_v<T> && !isMatrix_v<E>) {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return vecConcat(ConstVector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
				}
				else {
					return vecConcat(Vector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
				}
			}
			else {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return ConstVector<decltype(dot(t, e))>{};
				}
				else { 
					return Vector<decltype(dot(t, e))>{};
				}
			}
		}

	template <class T, class E, typename=std::enable_if_t<isVector_v<T> && isVector_v<E>>>
		constexpr auto matMul(T t, E e) {
			if constexpr(isEmpty_v<T> || isEmpty_v<E>) {
				return ConstVector<>{};
			}
			else if constexpr(isMatrix_v<T> && isMatrix_v<E>) {
				return vecConcat(matMulHelper(head(t), transpose(e)), matMulHelper(tail(t), transpose(e)));
			}
			else if constexpr(!isMatrix_v<T> && isMatrix_v<E>) {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return vecConcat(ConstVector<decltype(dot(t, head(transpose(e))))>{}, matMulHelper(t, tail(transpose(e))));
				}
				else {
					return vecConcat(Vector<decltype(dot(t, head(transpose(e))))>{}, matMulHelper(t, tail(transpose(e))));
				}
			}
			else if constexpr(isMatrix_v<T> && !isMatrix_v<E>) {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return vecConcat(ConstVector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
				}
				else {
					return vecConcat(Vector<decltype(dot(head(t), e))>{}, matMulHelper(tail(t), e));
				}
			}
			else {
				if constexpr(isConstant_v<T> && isConstant_v<E>) {
					return ConstVector<decltype(dot(t, e))>{};
				}
				else {
					return Vector<decltype(dot(t, e))>{};
				}
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

	template <class E, typename...Args>
		constexpr auto gradient(E e, Vector<Args...> v) {
			if constexpr(isVector_v<decltype(head(v))>) {
				return vecConcat(makeVector(gradient(head(v))), gradient(tail(v)));
			}
			else {
				return makeVector(e.getDerivative(Args{})...);
			}
		}

	template <class E, typename...Args>
		constexpr auto gradient(E e, ConstVector<Args...> v) {
			if constexpr(isVector_v<decltype(head(v))>) {
				return vecConcat(makeVector(gradient(head(v))), gradient(tail(v)));
			}
			else {
				return makeVector(e.getDerivative(Args{})...);
			}
		}

}


#endif
