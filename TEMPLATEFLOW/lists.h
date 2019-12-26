#ifndef LINALG_OPERATION_H_
#define LINALG_OPERATION_H_
#include <cmath>
#include <tuple>
#include <initializer_list>
#include <utility>

namespace TF {

	template <class T, size_t n>
		class Array {
		private:
			T data[n];
		public:
			constexpr Array() : data{} { }
			template <class E>
				constexpr Array(const std::initializer_list<E>& rhs) : data{} {
					auto b1 = rhs.begin();
					auto e1 = rhs.end();
					auto b2 = begin();
					auto e2 = end();
					while (b1 != e1 && b2 != e2) {
						*b2 = *b1;
						++b1;
						++b2;
					}
				}
			template <class E, size_t n2>
				constexpr Array(const Array<E, n2>& rhs) : data{} {
					auto b1 = rhs.begin();
					auto e1 = rhs.end();
					auto b2 = begin();
					auto e2 = end();
					while (b1 != e1 && b2 != e2) {
						*b2 = *b1;
						++b1;
						++b2;
					}
				}

			constexpr T* begin() {
				return data;
			}
			constexpr const T* begin() const {
				return data;
			}

			constexpr T* end() {
				return data+n;
			}
			constexpr const T* end() const {
				return data+n;
			}

			constexpr T& operator[](size_t i) {
				return data[i];
			}
			constexpr const T& operator[](size_t i) const {
				return data[i];
			}
		};

}


#endif
