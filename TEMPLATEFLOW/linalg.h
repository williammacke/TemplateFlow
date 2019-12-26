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
				using type = T;
				constexpr Tensor() = default;
				constexpr Tensor(const std::initializer_list<Tensor<T, dims...>>& rhs) : data(rhs) { }
				constexpr Tensor<T, dims...>& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const Tensor<T, dims...>& operator[](size_t ind) const {
					return data[ind];
				}

				constexpr Tensor<T, dims...>* begin() {
					//return data;
					return data.begin();
				}

				constexpr const Tensor<T, dims...>* begin() const {
					//return data;
					return data.begin();
				}

				constexpr Tensor<T, dims...>* end() {
					//return data+dim0;
					return data.end();
				}

				constexpr const Tensor<T, dims...>* end() const {
					//return data+dim0;
					return data.end();
				}
				constexpr T* beginRaw() {
					return data[0].beginRaw();
				}

				constexpr const T* beginRaw() const {
					return data[0].beginRaw();
				}

				constexpr T* endRaw() {
					return data[dim0-1].endRaw();
				}

				constexpr const T* endRaw() const{
					return data[dim0-1].endRaw();
				}
				template <size_t a = 0>
					constexpr static size_t size()  {
						if constexpr(a <= 0) {
							return dim0;
						}
						else {
							return Tensor<T, dims...>::template size<a-1>();
						}
					}

				constexpr static size_t numItems()  {
					return dim0*(dims*...);
				}

				constexpr Tensor<T, dims...>& operator() (size_t ind) { 
					return data[ind];
				}

				constexpr const Tensor<T, dims...>& operator() (size_t ind) const { 
					return data[ind];
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
		bool operator!=(const Tensor<F, dim1, dims1...>& lhs, const Tensor<E, dim2, dims2...>& rhs) {
			return !(lhs == rhs);
		}

	template <class T, size_t dim>
		using Vector = Tensor<T, dim>;

	template <class T, size_t dim>
		using rowVector = Tensor<T, 1, dim>;

	template <class T, size_t dim>
		using colVector = Tensor<T, dim, 1>;

	template <class T, size_t dim>
		class Tensor<T, dim> {
			friend class Tensor<T, 1, dim>;
			friend class Tensor<T, dim, 1>;
			private:
				Array<T, dim> data;
			public:
				using type = T;
				constexpr Tensor() = default;
				constexpr Tensor(const Array<T, dim>& rhs) : data(rhs) { }
				constexpr Tensor(const std::initializer_list<T>& rhs) : data(rhs) { }
				constexpr Tensor(const colVector<T, dim>& rhs) : data(static_cast<Array<T, dim>>(rhs.data)) { }
				constexpr explicit Tensor(const rowVector<T, dim>& rhs) : data(rhs(0).data) { }

				constexpr T& operator[](size_t ind) {
					return data[ind];
				}
				constexpr const T& operator[](size_t ind) const {
					return data[ind];
				}

				constexpr T& operator()(size_t ind) {
					return data[ind];
				}
				constexpr const T& operator()(size_t ind) const {
					return data[ind];
				}

				constexpr T* begin() {
					//return data;
					return data.begin();
				}

				constexpr const T* begin() const {
					//return data;
					return data.begin();
				}

				constexpr T* end() {
					//return data+dim;
					return data.end();
				}

				constexpr const T* end() const {
					//return data+dim;
					return data.end();
				};

				constexpr T* beginRaw() {
					return &data[0];
				}
				constexpr const T* beginRaw() const {
					return &data[0];
				}

				constexpr T* endRaw() {
					return data.end();
				}

				constexpr const T* endRaw() const {
					return data.end();
				}
				template <size_t a = 0>
					constexpr static size_t size()  {
						return dim;
					}
				constexpr static size_t numItems()  {
					return dim;
				}

		};

	template <class T, size_t dim>
		class Tensor<T, 1, dim> {
			friend class Tensor<T, dim>; 
			friend class Tensor<T, dim, 1>;
		private:
			Array<Tensor<T, dim>, 1> data;
		public:
			using type = T;
			constexpr Tensor() = default;
			constexpr Tensor(const std::initializer_list<T>& rhs) : data({rhs}) { }
			constexpr explicit Tensor(const Vector<T, dim>& rhs) : data({rhs.data}) { }
			constexpr explicit Tensor(const colVector<T, dim>& rhs) : data({rhs.data}) { }


			constexpr T& operator[](size_t i) {
				return data[0][i];
			}

			constexpr const T& operator[](size_t i) const {
				return data[0][i];
			}

			constexpr Tensor<T, dim>& operator()(size_t i) {
				return data[i];
			}

			constexpr const Tensor<T, dim>& operator()(size_t i) const {
				return data[i];
			}

			constexpr T* begin() {
				return data[0].begin();
			}
			constexpr const T* begin() const {
				return data[0].begin();
			}

			constexpr T* end() {
				return data[0].end();
			}

			constexpr const T* end() const {
				return data[0].end();
			}

			constexpr T* beginRaw() {
				return data[0].begin();
			}

			constexpr const T* beginRaw() const {
				return data[0].begin();
			}

			constexpr T* endRaw() {
				return data[0].end();
			}

			constexpr const T* endRaw() const {
				return data[0].end();
			}

			template <size_t a = 0>
				constexpr static size_t size() {
					return dim;
				}

			constexpr static size_t numItems() {
				return dim;
			}


		};

	template <class T, size_t dim>
		class Tensor<T, dim, 1> {
			friend class Tensor<T, dim>;
			friend class Tensor<T, 1, dim>;
		private:
			Array<Tensor<T, 1>, dim> data;
		public:	
			using type = T;
			constexpr Tensor() = default;
			constexpr Tensor(const std::initializer_list<T>& rhs) : data(rhs) { }
			constexpr Tensor(const Vector<T, dim>& rhs) : data(static_cast<Array<Tensor<T, 1>, dim>>(rhs.data)) { }
			constexpr explicit Tensor(const rowVector<T, dim>& rhs) : data(static_cast<Array<Tensor<T, 1>, dim>>(rhs.data)) { }


			constexpr T& operator[](size_t i) {
				return data[i][0];
			}

			constexpr const T& operator[](size_t i) const {
				return data[i][0];
			}


			constexpr Tensor<T, 1>& operator()(size_t i) {
				return data[i];
			}

			constexpr const Tensor<T, 1>& operator()(size_t i) const {
				return data[i];
			}

			constexpr T* begin() {
				return data[0].begin();
			}
			constexpr const T* begin() const {
				return data[0].begin();
			}

			constexpr T* end() {
				return data[dim-1].end();
			}

			constexpr const T* end() const {
				return data[dim-1].end();
			}

			constexpr T* beginRaw() {
				return data[0].begin();
			}

			constexpr const T* beginRaw() const {
				return data[0].begin();
			}

			constexpr T* endRaw() {
				return data[dim-1].end();
			}

			constexpr const T* endRaw() const {
				return data[dim-1].end();
			}
			template <size_t a = 0>
				constexpr static size_t size() {
					return dim;
				}

			constexpr static size_t numItems() {
				return dim;
			}

		};

	template <class T>
		class Tensor<T, 1> {
			private:
				//T data[dim] = {};
				T data = T();
			public:
				using type = T;
				constexpr Tensor() = default;

				constexpr operator T&() {
					return data;
				}
				constexpr operator T() const {
					return data;
				}

				constexpr Tensor(const T& rhs) : data(rhs) { }

				constexpr T& operator[](size_t ind) {
					return data;
				}
				constexpr const T& operator[](size_t ind) const {
					return data;
				}

				constexpr T& operator()(size_t ind) {
					return data;
				}
				constexpr const T& operator()(size_t ind) const {
					return data;
				}

				constexpr T* begin() {
					//return data;
					return &data;
				}

				constexpr const T* begin() const {
					//return data;
					return &data;
				}

				constexpr T* end() {
					//return data+dim;
					return &data+1;
				}

				constexpr const T* end() const {
					//return data+dim;
					return &data+1;
				};

				constexpr T* beginRaw() {
					return &data;
				}
				constexpr const T* beginRaw() const {
					return &data;
				}

				constexpr T* endRaw() {
					return &data+1;
				}

				constexpr const T* endRaw() const {
					return &data+1;
				}
				template <size_t a = 0>
					constexpr static size_t size()  {
						return 1;
					}
				constexpr static size_t numItems()  {
					return 1;
				}
		};



	template <class T>
		struct is_vector {
			constexpr static bool value = false;
		};

	template <class T, size_t dim>
		struct is_vector<Vector<T, dim>>  {
			constexpr static bool value = true;
		};

	template <class T, size_t dim>
		struct is_vector<rowVector<T, dim>> {
			constexpr static bool value = true;
		};
	template <class T, size_t dim>
		struct is_vector<colVector<T, dim>> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool is_vector_v = is_vector<T>::value;


	template <class T, class E, size_t dim, size_t...ind>
		constexpr auto dotHelper(const Vector<T, dim>& lhs, const Vector<E, dim>& rhs, std::integer_sequence<size_t, ind...> seq) {
			return ((lhs[ind]*rhs[ind])+...);
		}

	template <class V1, class V2,
		 typename=std::enable_if_t<is_vector_v<V1> && is_vector_v<V2> && V1::size() == V2::size() &&
			 !(std::is_same_v<V1, colVector<typename V1::type, V1::size()>> && std::is_same_v<V2, rowVector<typename V2::type, V2::size()>>) &&
			 !(std::is_same_v<V1, Vector<typename V1::type, V1::size()>> && std::is_same_v<V2, rowVector<typename V2::type, V2::size()>>)>
		>
		constexpr auto operator*(const V1& lhs, const V2& rhs) {
			return dotHelper(Vector<typename V1::type, V1::size()>(lhs), Vector<typename V2::type, V2::size()>(rhs), std::make_index_sequence<V1::size()>());
		}

	template <class V1, class V2,
		 typename=std::enable_if_t<is_vector_v<V1> && is_vector_v<V2> && V1::size() == 3 && V2::size() == 3>>
			 constexpr auto operator^(const V1& lhs, const V2& rhs) {
				 return Vector<decltype(lhs[0]*rhs[0]), 3>{lhs[1]*rhs[2]-lhs[2]*rhs[1], lhs[2]*rhs[0]-lhs[0]*rhs[2], lhs[0]*rhs[1] - lhs[1]*rhs[0]};
			 }

	template <class T, size_t i, size_t j>
		using Mat = Tensor<T, i, j>;

	template <size_t n, class T=float>
		constexpr auto I() {
			Mat<T, n, n> Id;
			for (size_t i = 0; i < n; i++) {
				Id[i][i] = T(1);
			}
			return Id;
		}


	template <class T, size_t i, size_t j>
		constexpr Mat<T, j, i> trans(const Mat<T, i , j>& val) {
			Mat<T, j, i> mat;
			for (int r = 0; r < i; r++) {
				for (int c = 0; c < j; c++) {
					mat(c)(r) = val(r)(c);
				}
			}
			return mat;

		}

	template <class T, size_t i>
		constexpr Mat<T, 1, i> trans(const Vector<T, i>& val) {
			Mat<T, 1, i> mat;
			for (int r = 0; r < i; r++) {
				mat(0)(r) = val[r];
			}
			return mat;
		}

	template <class T, class E, size_t dim, size_t i, size_t j, size_t...ind>
		constexpr auto matMulHelper(const Vector<T, dim>& lhs, const Mat<E, i, j>& rhs, std::integer_sequence<size_t, ind...>) {
			return Vector<decltype(lhs(0)*rhs(0)(0)), i>({lhs*rhs(ind)...});
		}

	template <class T, class E, size_t i, size_t j, size_t k, size_t...ind>
		constexpr auto matMulHelper(const Mat<T, i, j>& lhs, const Mat<E, k, j>& rhs, std::integer_sequence<size_t, ind...>) {
			return Mat<decltype(lhs(0)*rhs(0)), i, k>({matMulHelper(lhs(ind), rhs, std::make_index_sequence<k>())...});
		}

	template <class T, class E, size_t i, size_t j, size_t k, typename=std::enable_if_t<i != 1 || k != 1>>
		constexpr auto operator*(const Mat<T, i, j>& lhs, const Mat<E, j, k>& rhs) {
			return matMulHelper(lhs, trans(rhs), std::make_index_sequence<i>());
		}

	template <class T, class E, size_t i, size_t j>
		constexpr auto operator*(const Mat<T, i ,j>& lhs, const Vector<E, j>& rhs) {
			return matMulHelper(rhs, lhs, std::make_index_sequence<i>());
		}

	template <class F, class E, size_t dim1, size_t dim2, size_t...dims1, size_t...dims2>
		constexpr bool operator==(const Tensor<F, dim1, dims1...>& lhs, const Tensor<E, dim2, dims2...>& rhs) {
			if constexpr(is_vector_v<Tensor<F, dim1, dims1...>> && is_vector_v<Tensor<E, dim2, dims2...>>) {
				if constexpr(lhs.numItems() != rhs.numItems()) {
					return false;
				}
				else {
					auto b1 = lhs.beginRaw();
					auto e1 = lhs.endRaw();
					auto b2 = rhs.beginRaw();
					while (b1 != e1) {
						if (*b1 != *b2) {
							return false;
						}
						++b1;
						++b2;
					}
					return true;
				}
			}
			else {
				if constexpr(dim1 != dim2) {
					return false;
				}
				else {
					auto b1 = lhs.begin();
					auto e1 = lhs.end();
					auto b2 = rhs.begin();
					while (b1 != e1) {
						if (*b1 != *b2) {
							return false;
						}
						++b1;
						++b2;
					}
					return true;
				}
			}
		}

	template <class F, class E, size_t dim1, size_t dim2>
		constexpr bool operator!=(const Tensor<F, dim1>& lhs, const Tensor<E, dim2>& rhs) {
			return !(lhs == rhs);
		}


}

#endif
