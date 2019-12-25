#ifndef LINALG_H_
#define LINALG_H_
#include "lists.h"
namespace TF{
	template <class T, size_t dim0, size_t...dims>
		class Tensor {
			private:
				//Tensor<T, dims...> data[dim0];
				Array<Tensor<T, dims...>, dim0> data;
			public:
				constexpr Tensor() = default;
				constexpr Tensor(const std::initializer_list<Tensor<T, dims...>>& rhs) : data(rhs) { }
				constexpr Tensor<T, dims...>& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const Tensor<T, dims...>& operator[](size_t ind) const {
					return data[ind];
				}

				Tensor<T, dims...>* begin() {
					//return data;
					return data.begin();
				}

				const Tensor<T, dims...>* begin() const {
					//return data;
					return data.begin();
				}

				Tensor<T, dims...>* end() {
					//return data+dim0;
					return data.end();
				}

				const Tensor<T, dims...>* end() const {
					//return data+dim0;
					return data.end();
				}
		};

	template <class T>
		struct is_scalar {
			constexpr static bool value = true;
		};

	template <class T, size_t...dims>
		struct is_scalar<Tensor<T, dims...>> {
			constexpr static bool value = false;
		};

	template <class T>
		constexpr bool is_scalar_v = is_scalar<T>::value;


	template <class F, class E, size_t dim1, size_t dim2, size_t... dims1, size_t...dims2>
		bool operator==(const Tensor<F, dim1, dims1...>& lhs, const Tensor<E, dim2, dims2...>& rhs) {
			return dim1==dim2 && std::equal(lhs.begin(), lhs.end(), rhs.begin());
		}

	template <class F, class E, size_t dim1, size_t dim2, size_t... dims1, size_t...dims2>
		bool operator!=(const Tensor<F, dim1, dims1...>& lhs, const Tensor<E, dim2, dims2...>& rhs) {
			return !(lhs == rhs);
		}

	template <class T, size_t dim>
		class Tensor<T, dim> {
			private:
				//T data[dim] = {};
				Array<T, dim> data;
			public:
				constexpr Tensor() = default;
				/*
				constexpr Tensor(const std::initializer_list<T>& rhs)  { 
					auto b = rhs.begin();
					auto e = rhs.end();
					auto o = std::begin(data);
					auto e2 = std::end(data);
					while (b != e && o != e2) {
						*o = *b;
						++b;
						++o;
					}
				}
				*/
				constexpr Tensor(const std::initializer_list<T>& rhs) : data(rhs) { }

				constexpr T& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const T& operator[](size_t ind) const {
					return data[ind];
				}

				T* begin() {
					//return data;
					return data.begin();
				}

				const T* begin() const {
					//return data;
					return data.begin();
				}

				T* end() {
					//return data+dim;
					return data.end();
				}

				const T* end() const {
					//return data+dim;
					return data.end();
				};

		};
	template <class F, class E, size_t dim1, size_t dim2>
		bool operator==(const Tensor<F, dim1>& lhs, const Tensor<E, dim2>& rhs) {
			return dim1==dim2&&std::equal(lhs.begin(), lhs.end(), rhs.begin());
		}

	template <class F, class E, size_t dim1, size_t dim2>
		bool operator!=(const Tensor<F, dim1>& lhs, const Tensor<E, dim2>& rhs) {
			return !(lhs == rhs);
		}

	template <class T, size_t dim>
		using Vector = Tensor<T, dim>;

	template <class T, size_t dim>
		using rowVector = Tensor<T, 1, dim>;

	template <class T, class E, size_t dim, size_t...ind>
		constexpr auto dotHelper(const Vector<T, dim>& lhs, const Vector<E, dim>& rhs, std::integer_sequence<size_t, ind...> seq) {
			return ((lhs[ind]*rhs[ind])+...);
		}

	template <class T, class E, size_t dim>
		constexpr auto operator*(const Vector<T, dim>& lhs, const Vector<E, dim>& rhs) {
			return dotHelper(lhs, rhs, std::make_index_sequence<dim>());
		}

	template <class T, size_t i, size_t j>
		using Mat = Tensor<T, i, j>;


	template <class T, size_t i, size_t j>
		constexpr Mat<T, j, i> trans(const Mat<T, i , j>& val) {
			Mat<T, j, i> mat;
			for (int r = 0; r < i; r++) {
				for (int c = 0; c < j; c++) {
					mat[c][r] = val[r][c];
				}
			}
			return mat;

		}

	template <class T, size_t i>
		constexpr Mat<T, 1, i> trans(const Vector<T, i>& val) {
			Mat<T, 1, i> mat;
			for (int r = 0; r > i; r++) {
				mat[0][r] = val[r];
			}
			return mat;
		}

	template <class T, class E, size_t dim, size_t i, size_t j, size_t...ind>
		constexpr auto matMulHelper(const Vector<T, dim>& lhs, const Mat<E, i, j>& rhs, std::integer_sequence<size_t, ind...>) {
			return Vector<decltype(lhs[0]*rhs[0][0]), i>({lhs*rhs[ind]...});
		}

	template <class T, class E, size_t i, size_t j, size_t k, size_t...ind>
		constexpr auto matMulHelper(const Mat<T, i, j>& lhs, const Mat<E, k, j>& rhs, std::integer_sequence<size_t, ind...>) {
			return Mat<decltype(lhs[0]*rhs[0]), i, k>({matMulHelper(lhs[ind], rhs, std::make_index_sequence<k>())...});
		}

	template <class T, class E, size_t i, size_t j, size_t k>
		constexpr auto operator*(const Mat<T, i, j>& lhs, const Mat<E, j, k>& rhs) {
			return matMulHelper(lhs, trans(rhs), std::make_index_sequence<i>());
		}

	template <class T, class E, size_t i, size_t j>
		constexpr auto operator*(const Mat<T, i ,j>& lhs, const Vector<E, j>& rhs) {
			return matMulHelper(rhs, lhs, std::make_index_sequence<i>());
		}
}

#endif
