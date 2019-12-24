#ifndef LINALG_H_
#define LINALG_H_
#include <array> 
namespace TF{
	template <class T, size_t dim0, size_t...dims>
		class Tensor {
			private:
				Tensor<T, dims...> data[dim0];
			public:
				constexpr Tensor() = default;
				constexpr Tensor(const std::initializer_list<Tensor<T, dims...>>& rhs) {
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
				constexpr Tensor<T, dims...>& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const Tensor<T, dims...>& operator[](size_t ind) const {
					return data[ind];
				}

				Tensor<T, dims...>* begin() {
					return data;
				}

				const Tensor<T, dims...>* begin() const {
					return data;
				}

				Tensor<T, dims...>* end() {
					return data+dim0;
				}

				const Tensor<T, dims...>* end() const {
					return data+dim0;
				}
		};
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
				T data[dim] = {};
			public:
				constexpr Tensor() = default;
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
				constexpr T& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const T& operator[](size_t ind) const {
					return data[ind];
				}

				T* begin() {
					return data;
				}

				const T* begin() const {
					return data;
				}

				T* end() {
					return data+dim;
				}

				const T* end() const {
					return data+dim;
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
}

#endif
