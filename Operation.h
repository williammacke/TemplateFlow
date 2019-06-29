#ifndef OPERATION_H_
#define OPERATION_H_
#include <memory>
#include <type_traits>
#include <cmath>
#include "math.h"

namespace TF {



	template <class T, T val>
		struct Val {
			constexpr static T getVal() {
				return val;
			}

			
			template <class E>
				constexpr static Val<T, T{}> getDerivative(E var) {
					return {};
				}
		};

	template <class T>
		struct isConstant {
			constexpr static bool value = false;
		};

	template <class T, T val>
		struct isConstant<Val<T, val>> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool isConstant_v = isConstant<T>::value;


	template <long num, long dem>
		struct Double {
			static constexpr double val = static_cast<double>(num)/dem;
		};


	template <long num, long dem>
		struct Float {
			constexpr static float val = static_cast<float>(num)/dem;
		};


	template <class T>
		struct FVal;

	template <long num, long dem>
		struct FVal<Float<num,dem>> {
			constexpr static float getVal() {
				return Float<num,dem>::val;
			}

			template <class V>
			constexpr static Float<0, 1> getDerivative(V var) {
				return {};
			}


		};

	template <long num, long dem>
		struct isConstant<FVal<Float<num, dem>>> {
			static constexpr bool value = true;
		};
	template<long i, long j>
		struct FVal<Double<i, j>> {
			static constexpr double getVal() {
				return Double<i,j>::val;
			}

			template <class V>
			constexpr static Double<0, 1> getDerivative(V var) {
				return {};
			}
		};

	template <long i, long j>
		struct isConstant<FVal<Double<i, j>>> {
			constexpr static bool value = true;
		};
		



	template <class T, T* var>
		struct Var {
			static T getVal() {
				return *var;
			}

			template <class E>
				constexpr static Val<T, T{}> getDerivative(E v) {
					return {};
				}

			constexpr static Val<T, 1> getDerivative(Var<T, var> v) {
				return {};
			}

			template <class E>
			static void addVal(E val) {
				*var += val;
			}
		};

	template <float *var>
		struct Var<float, var> {
			static float getVal() {
				return *var;
			}

			template <class E>
				constexpr static FVal<Float<0,1>> getDerivative(E v) {
					return {};
				}

			constexpr static FVal<Float<1,1>> getDerivative(Var<float, var> v) {
				return {};
			}

			template <class E>
				static void addVal(E val) {
					*var += val;
				}
		};


	template <class T>
		constexpr auto isOperation(T t) -> decltype(&T::getVal,T::getDerivative(Var<int, nullptr>{}),std::true_type{}) {
			return {};
		}

	constexpr auto isOperation(...) -> std::false_type {
		return {};
	}

	template <class T, T* var>
		struct Placeholder {
			static T getVal() {
				return *var;
			}

			template <class E>
				constexpr static Val<T, T{}> getDerivative(E v) {
					return {};
				}

			template <class E> 
				static void setVal(E val) {
					*var = val;
				}
		};

	template <float* var>
		struct Placeholder<float, var> {
			static float getVal() {
				return *var;
			}

			template <class V>
				constexpr static FVal<Float<0, 1>> getDerivative(V v) {
					return {};
				}

			template <class E>
				static void setVal(E val) {
					*var = val;
				}
		};

	template <double* var>
		struct Placeholder<double, var> {
			static double getVal() {
				return *var;
			}

			template <class V>
				constexpr static FVal<Double<0, 1>> getDerivative(V v) {
					return {};
				}
			template <class E>
				static void setVal(E val) {
					*var = val;
				}
		};

	template <class T, class E>
		struct Addition {
			static auto getVal() -> decltype(T::getVal() + E::getVal()) {
				return T::getVal()+E::getVal();
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Addition<decltype(T::getDerivative(var)), decltype(E::getDerivative(var))> {
				return {};
			}

		};

	template <class T, class E>
		struct ConstAddition {
			constexpr static auto getVal() -> decltype(T::getVal()+E::getVal()) {
				return T::getVal()+E::getVal();
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Val<int, 0> {
					return {};
				}
		};

	template <class T, class E>
		struct isConstant<ConstAddition<T, E>> {
			constexpr static bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		constexpr auto operator+(T t, E e) {
			if constexpr(isConstant_v<T> && isConstant_v<E>) {
				return ConstAddition<T, E>{};
			}
			else {
				return Addition<T, E>{};
			}
		}

	template <class T, class E>
		struct Subtraction {
			static auto getVal() -> decltype(T::getVal() - E::getVal()) {
				return T::getVal() - E::getVal();
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Subtraction<decltype(T::getDerivative(var)), decltype(E::getDerivative(var))> {
					return {};
				}
		};

	template <class T, class E>
		struct ConstSubtraction {
			constexpr static auto getVal() -> decltype(T::getVal()-E::getVal()) {
				return T::getVal()-E::getVal();
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Val<int, 0> {
					return {};
				}
		};

	template <class T, class E>
		struct isConstant<ConstSubtraction<T, E>> {
			static constexpr bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		constexpr auto operator-(T t, E e) {
			if constexpr(isConstant_v<T> && isConstant_v<E>) {
				return ConstSubtraction<T, E>{};
			}
			else {
				return Subtraction<T,E>{};
			}
		}

	template <class T, class E>
		struct Multiplication {
			static auto getVal() -> decltype(T::getVal()*E::getVal()) {
				return T::getVal() * E::getVal();
			}

			template <class V>
				constexpr static auto getDerivative(V var) -> Addition<Multiplication<T, decltype(E::getDerivative(var))>,
					    					Multiplication<decltype(T::getDerivative(var)), E>> {
											return {};
										}
		};

	template <class T, class E>
		struct ConstMultiplication {
			static constexpr auto getVal() -> decltype(T::getVal()*E::getVal()) {
				return T::getVal()*E::getVal();
			}

			template <class V>
				static constexpr auto getDerivative(V var) -> Val<int,0> {
									return {};
				}
		};

	template <class T, class E>
		struct isConstant<ConstMultiplication<T, E>> {
			constexpr static bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		constexpr auto operator*(T t, E e) {
			if constexpr(isConstant_v<T> && isConstant_v<E>) {
				return ConstMultiplication<T, E>{};
			}
			else {
				return Multiplication<T, E>{};
			}
		}

	template <class T, class E>
		struct exp;

	template <class T, class E>
		struct Division {
			static auto getVal() -> decltype(T::getVal()/E::getVal()) {
				return E::getVal()==0?0:T::getVal()/E::getVal();
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Division<
								Subtraction<Multiplication<E, decltype(T::getDerivative(var))>,
										Multiplication<decltype(E::getDerivative(var)), T>>,
										exp<E, Val<int, 2>>> {
											return {};
										}
		};


	template <class T, class E>
		struct ConstDivision {
			static constexpr decltype(T::getVal()/E::getVal()) getVal() {
				return T::getVal()/E::getVal();
			}

			template <class V>
				static constexpr Val<int, 0> getDerivative(V var) {
					return {};
				}

		};

	template <class T, class E>
		struct isConstant<ConstDivision<T, E>> {
			static constexpr bool value = true;
		};

	template <class T>
		struct ln;
	template <class T, class E>
		struct log;

	template <class T, class E>
		struct exp {
			static auto getVal() -> decltype(pow(T::getVal(), E::getVal())) {
				return pow(T::getVal(), E::getVal());
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Multiplication<exp<T,E>, Addition<
										Multiplication<decltype(E::getDerivative(var)), ln<T>>,
										Multiplication<decltype(T::getDerivative(var)), Division<E, T>>>> {
											return {};
										}
											
		};

	template <class T, class E>
		struct ConstExp {
			static constexpr decltype(math::pow(T::getVal(), E::getVal())) getVal() {
				return math::pow(T::getVal(), E::getVal());
			}

			template <class V>
				static constexpr Val<int, 0> getDerivative(V var) {
					return {};
				}
		};

	template <class T, class E>
		struct isConstant<ConstExp<T, E>> {
			static constexpr bool value = true;
		};


	template <class T>
		struct ln {
			static double getVal() {
				return T::getVal()<=0?0:std::log(T::getVal());
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Division<FVal<Float<1,1>>, T> {
				return {};
			}
		};

	template <class T>
		struct Constln {
			static constexpr decltype(math::log(T::getVal())) getVal() {
				return math::log(T::getVal());
			}

			template <class V>
				static constexpr Val<int, 0> getDerivative(V var) {
					return {};
				}
		};

	template <class T>
		struct isConstant<Constln<T>> {
			static constexpr bool value = true;
		};

	template <class T, class E>
		struct log {
			static auto getVal() -> decltype(std::log(E::getVal())/std::log(T::getVal())) {
				return std::log(E::getVal())/std::log(T::getValI());
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Division<FVal<Float<1,1>>, Multiplication<E, ln<T>>> {
				return {};
			}
		};

	template <class T, class E>
		struct ConstLog {
			static constexpr decltype(math::log(E::getVal())/math::log(T::getVal())) getVal() {
				return math::log(E::getVal())/math::log(T::getVal());
			}

			template <class V>
				static constexpr Val<int, 0> getDerivative(V var) {
					return {};
				}
		};

	template <class T, class E>
		struct isConstant<ConstLog<T,  E>> {
			static constexpr bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		constexpr auto operator/(T t, E e) {
			if constexpr(isConstant_v<T> && isConstant_v<E>) {
				return ConstDivision<T, E>{};
			}
			else {
				return Division<T, E>{};
			}
		}
	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		constexpr auto operator^(T t, E e) {
			if constexpr(isConstant_v<T> && isConstant_v<E>) {
				return ConstExp<T, E>{};
			}
			else { 
				return exp<T, E>{};
			}
		}

	template <class T, class E>
		struct func {
			static auto getVal() -> decltype(T{}(E::getVal())) {
				return T{}(E::getVal());
			}

			template <class V>
			constexpr static auto getDerivative(V var) -> Multiplication<func<decltype(T::getDerivative(var)), E>, decltype(E::getDerivative(var))> {
				return {};
			}
		};

	template <class T, class E>
		struct ConstFunc {
			static constexpr decltype(T{}(E{})) getVal() {
				return T{}(E{});
			}

			template <class V>
				static constexpr Val<int, 0> getDerivative(V var) {
					return {};
				}
		};


	template <class T, int=(T{}(0),0)>
		constexpr bool is_constexpr(T t) {
			return true;
		}

	template <class T, int i =1, int=(T{}(0,0),0)>
		constexpr bool is_constexpr(T t) {
			return true;
		}

	constexpr bool is_constexpr(...) {
		return false;
	}

	template <class T, class E>
		struct isConstant<ConstFunc<T, E>> {
			static constexpr bool value = true;
		};
}
#endif
