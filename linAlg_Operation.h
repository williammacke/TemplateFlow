#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include "Operation.h"
namespace ML {


	template <int r, int c, class T, T arg1,  T...Args>
		struct MatrixVal {
			static constexpr T data[] = {arg1, Args...};

			static const T* getVal() {
				return data;
			}

			template <class V>
				static MatrixVal<r, c, T, 0, 0*Args...> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};


	template <int r, int c, class T, T arg1>
		struct MatrixVal<r, c, T, arg1> {
			static constexpr T data[r*c] = {arg1};

			static const T* getVal() {
				return data;
			}

			template <class V>
				static MatrixVal<r, c, T, 0> getDerivative(V var) {
					return {};
				}

			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c, class T>
		struct Identity {
			static const T* getVal() {
				return nullptr;
			}

			template <class V>
				static MatrixVal<r, c, T, 0> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <class T>
		struct isIdentity {
			constexpr static bool value = false;
		};

	template <int r, int c, class T>
		struct isIdentity<Identity<r,c,T>> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool isIdentity_v = isIdentity<T>::value;

	template <int r, int c,typename...Args>
		struct MatrixFVal;

	template <long i, long j>
		constexpr Float<0,1> Float2Zero(Float<i,j>) {
			return {};
		}

	template <long i, long j>
		constexpr Double<0,1> Double2Zero(Double<i,j>) {
			return {};
		}

	template <int r, int c, long i, long j, typename...Args>
		struct MatrixFVal<r, c, Float<i,j>, Args...> {
			static constexpr float data[] = {Float<i,j>::val, Args::val...};

			static const float* getVal() {
				return data;
			}

			template <class V>
				static MatrixFVal<r, c, Float<0,1>, decltype(Float2Zero(Args{}))...> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c>
		struct MatrixFVal<r, c, Float<0,1>> {
			static constexpr float data[r*c] = { 0.0f };	

			static const float* getVal() {
				return data;
			}

			template <class V>
				static MatrixFVal<r, c, Float<0,1>> getDerivative(V var) {
					return {};
				}

			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c>
		struct Identity<r, c, float> {
			static const float* getVal() {
				return nullptr;
			}

			template <class V>
				static MatrixFVal<r, c, Float<0,1>> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c, long i, long j, typename...Args>
		struct MatrixFVal<r, c, Double<i, j>, Args...> {
			static constexpr double data[] = {Double<i,j>::val, Args::val...};

			static const double* getVal() {
				return data;
			}

			template <class V>
				static MatrixFVal<r, c, Double<0,1>, decltype(Double2Zero(Args{}))...> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c>
		struct MatrixFVal<r, c, Double<0,1>> {
			static constexpr double data[r*c] = {0.0};

			static const double* getVal() {
				return data;
			}

			template <class V>
				static MatrixFVal<r, c, Double<0,1>> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c>
		struct Identity<r, c, double> {
			static const double* getVal() {
				return nullptr;
			}

			template <class V>
				static MatrixFVal<r, c, Double<0,1>> getDerivative(V var) {
					return {};
				}
			constexpr static int rows = r;
			constexpr static int cols = c;
		};

	template <int r, int c, class T, T* data>
		struct MatrixVar {
			static T* getVal() {
				return data;
			}

			template <class V>
				static MatrixVal<r, c, T, 0> getDerivative(V var) {
					return {};
				}

			static Identity<r, c, T> getDerivative(MatrixVar<r, c, T, data>) {
				return {};
			}

		};

	template <class T, class E, typename=std::enable_if_t<T::rows==E::rows && T::cols==E::cols>>
		struct MatrixAdd {
			static decltype(T::getVal()[0]+E::getVal()[0]) data[T::rows*T::cols] = {0};

			static decltype(data) getVal() {
				if constexpr(isIdentity_v<T> && isIdentity_v<E>) {
					if (data[0] == 2) {
						return data;
					}
					for (int i = 0; i < T::cols; i++) {
						for (int j = 0; j < T::rows; j++) {
							if (i == j) {
								data[j+i*T::rows] = 2;
							}
						}
					}
					return data;
				}
				else if constexpr(isIdentity_v<T>) {
					auto a = E::getVal();
					for (int i = 0; i < T::cols; i++) {
						for (int j = 0; j < T::rows; j++) {
							if (i == j) {
								data[j+i*T::rows] = a[j+i*T::rows]+1;
							}
							else {
								data[j+i*T::rows] = a[j+i*T::rows];
							}
						}
					}
					return data;
				}
				else if constexpr(isIdentity_v<E>) {
					auto b = T::getVal();
					for (int i = 0; i < T::cols; i++) {
						for (int j = 0; j < T::rows; j++) {
							if (i == j) {
								data[j+i*T::rows] = b[j+i*T::rows]+1;
							}
							else {
								data[j+i*T::rows] = b[j+i*T::rows];
							}
						}
					}
					return data;
				}
				else {
					auto a = T::getVal();
					auto b = E::getVal();
					for (int i = 0; i < T::rows*T::cols; i++) {
						data[i] = a[i]+b[i];
					}
					return data;
				}
			}

			template <class V>
				static auto getDerivative(V var) -> MatrixAdd<decltype(T::getDerivative(var)), decltype(E::getDerivative(var))> {
					return {};
				}
		};
}


#endif
