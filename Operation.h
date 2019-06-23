#ifndef OPERATION_H_
#define OPERATION_H_
#include <memory>
#include <type_traits>
#include <cmath>

namespace ML {



	template <class T, T val>
		struct Val {
			static T getVal() {
				return val;
			}

			
			template <class E>
				static Val<T, T{}> getDerivative(E var) {
					return {};
				}
		};


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
			static float getVal() {
				return Float<num,dem>::val;
			}

			template <class V>
			static Float<0, 1> getDerivative(V var) {
				return {};
			}


		};
	template<long i, long j>
		struct FVal<Double<i, j>> {
			static double getVal() {
				return Double<i,j>::val;
			}

			template <class V>
			static Double<0, 1> getDerivative(V var) {
				return {};
			}
		};

	template <class T, T* var>
		struct Var {
			static T getVal() {
				return *var;
			}

			template <class E>
				static Val<T, T{}> getDerivative(E v) {
					return {};
				}

			static Val<T, 1> getDerivative(Var<T, var> v) {
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
				static FVal<Float<0,1>> getDerivative(E v) {
					return {};
				}

			static FVal<Float<1,1>> getDerivative(Var<float, var> v) {
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
				static Val<T, T{}> getDerivative(E v) {
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
				static FVal<Float<0, 1>> getDerivative(V v) {
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
				static FVal<Double<0, 1>> getDerivative(V v) {
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
			static auto getDerivative(V var) -> Addition<decltype(T::getDerivative(var)), decltype(E::getDerivative(var))> {
				return {};
			}

		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		Addition<T, E> operator+(const T& t, const E& e) {
			return {};
		}

	template <class T, class E>
		struct Subtraction {
			static auto getVal() -> decltype(T::getVal() - E::getVal()) {
				return T::getVal() - E::getVal();
			}

			template <class V>
				static auto getDerivative(V var) -> Subtraction<decltype(T::getDerivative(var)), decltype(E::getDerivative(var))> {
					return {};
				}
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		Subtraction<T, E> operator-(const T& t, const E& e) {
			return {};
		}

	template <class T, class E>
		struct Multiplication {
			static auto getVal() -> decltype(T::getVal()*E::getVal()) {
				return T::getVal() * E::getVal();
			}

			template <class V>
				static auto getDerivative(V var) -> Addition<Multiplication<T, decltype(E::getDerivative(var))>,
					    					Multiplication<decltype(T::getDerivative(var)), E>> {
											return {};
										}
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		Multiplication<T, E> operator*(const T& t, const E& e) {
			return {};
		}

	template <class T, class E>
		struct exp;

	template <class T, class E>
		struct Division {
			static auto getVal() -> decltype(T::getVal()/E::getVal()) {
				return T::getVal()/E::getVal();
			}

			template <class V>
			static auto getDerivative(V var) -> Division<
								Subtraction<Multiplication<E, decltype(T::getDerivative(var))>,
										Multiplication<decltype(E::getDerivative(var)), T>>,
										exp<E, Val<int, 2>>> {
											return {};
										}
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
			static auto getDerivative(V var) -> Multiplication<exp<T,E>, Addition<
										Multiplication<decltype(E::getDerivative(var)), ln<T>>,
										Multiplication<decltype(T::getDerivative(var)), Division<E, T>>>> {
											return {};
										}
											
		};

	template <class T>
		struct ln {
			static double getVal() {
				return T::getVal()<=0?0:std::log(T::getVal());
			}

			template <class V>
			static auto getDerivative(V var) -> Division<FVal<Float<0,1>>, T> {
				return {};
			}
		};

	template <class T, class E>
		struct log {
			static auto getVal() -> decltype(log(E::getVal())/log(T::getVal())) {
				return log(E::getVal())/log(T::getValI());
			}

			template <class V>
			static auto getDerivative(V var) -> Division<FVal<Float<0,1>>, Multiplication<E, ln<T>>> {
				return {};
			}
		};

	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		Division<T, E> operator/(const T& t, const E& e) {
			return {};
		}
	template <class T, class E, typename=std::enable_if_t<isOperation(T{}) && isOperation(E{})>>
		exp<T, E> operator^(const T& t, const E& e) {
			return {};
		}

	template <class T, class E>
		struct func {
			static auto getVal() -> decltype(T{}(E::getVal())) {
				return T{}(E::getVal());
			}

			template <class V>
			static auto getDerivative(V var) -> Multiplication<func<decltype(T::getDerivative(var)), E>, decltype(E::getDerivative(var))> {
				return {};
			}
		};



}
#endif
