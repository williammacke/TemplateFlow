#ifndef VARIABLE_H_
#define VARIABLE_H_
#include "linalg.h"
#include <memory>
#include <type_traits>
#include <cmath>
#include <unordered_map>
#include <string>
#include <sstream>
#include "math.h"
#include "include/gcem.hpp"
#include <tuple>

namespace TF {

	struct Zero {
		constexpr static int getVal() {
			return 0;
		}

		template <class V>
			constexpr static Zero getDerivative(const V& var) {
				return {};
			}

		constexpr operator int() const {
			return 0;
		}

		std::string getName() const {
			return "0";
		}

		constexpr static auto getVars() {
			return std::make_tuple();
		}


	};

	struct Id {
		constexpr static int getVal() {
			return 1;
		}

		template <class V>
			constexpr static Zero getDerivative(const V& var) {
				return {};
			}

		constexpr operator int() const {
			return 1;
		}

		std::string getName() const {
			return "1";
		}

		constexpr static auto getVars() {
			return std::make_tuple();
		}
	};

	template <class T>
		struct Value {
			const T val;
			template <class E>
				constexpr Value(const E& val) : val(val) { }
			constexpr Value(const Value& rhs) : val(rhs.val) { }


			constexpr T getVal() const {
				return val;
			}
			template <class V>
				constexpr Zero getDerivative(const V& var) const {
					return {};
				}

			constexpr operator T() const {
				return val;
			}

			std::string getName() const {
				std::stringstream s;
				s << val;
				return s.str();
			}

			constexpr static auto getVars() {
				return std::make_tuple();
			}

		};

	template <class T>
		struct is_value {
			constexpr static bool value = false;
		};

	template <class T>
		struct is_value<Value<T>> {
			constexpr static bool value = true;
		};
	template <class T>
		constexpr bool is_value_v = is_value<T>::value;

	template <class T>
		struct is_operation {
			constexpr static bool value = false;
		};

	template <class T>
		struct is_operation<Value<T>> {
			constexpr static bool value = true;
		};

	template <>
		struct is_operation<Zero> {
			constexpr static bool value = true;
		};

	template <>
		struct is_operation<Id> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool is_operation_v = is_operation<T>::value;

	template <class T>
		struct is_scalar<Value<T>> {
			constexpr static bool value = is_scalar<T>::value;
		};

	template <>
		struct is_scalar<Zero> {
			constexpr static bool value = true;
		};

	template <>
		struct is_scalar<Id> {
			constexpr static bool value = true;
		};

	template <class T>
		struct is_vector<Value<T>> {
			constexpr static bool value = is_vector<T>::value;
		};

	template <class T>
		constexpr auto value(const T& val) {
			if constexpr(is_operation_v<T>) {
				return val;
			}
			else {
				return Value<T>(val);
			}
		}



	template <char...chars>
		using t_string=std::integer_sequence<char, chars...>;

	template <typename T, T...chars>
		constexpr t_string<chars...> operator""_tstr() {
			return {};
		}

	template <class T,  char...name>
		struct Var {
			static T val;
			constexpr Var() { }

			static T getVal()  {
				return val;
			}


			const Var& operator=(const T& rhs) const {
				val = rhs;
				return *this;
			}
			Var& operator=(const T& rhs) {
				val = rhs;
				return *this;
			}
			template <class V>
				constexpr auto getDerivative(const V& var) const {
					if constexpr(std::is_same_v<V, Var<T, name...>>) {
						//return value(1);
						return Id();
					}
					else {
						//return value(0);
						return Zero();
					}
				}

			static std::string getName() {
				static char buffer[] = {name..., '\0'};
				return buffer;
			}

			operator T() const {
				return val;
			}

			static constexpr auto getVars() {
				return std::make_tuple(Var{});
			}
		};

	template <class T, char...name>
		struct is_operation<Var<T, name...>> {
			constexpr static bool value = true;
		};


	template <class T, char...name>
		struct is_scalar<Var<T, name...>> {
			constexpr static bool value = is_scalar<T>::value;
		};

	template <class T, char...name>
		struct is_vector<Var<T, name...>> {
			constexpr static bool value = is_vector<T>::value;
		};



	template <class T, char...name>
		T Var<T, name...>::val = T();


	template <class T = int, char...name>
		constexpr auto Variable(t_string<name...>) {
			return Var<T, name...>();
		}

	template <class T>
		struct is_const {
			constexpr static bool value = is_value_v<T> || !is_operation_v<T>;
		};

	template <>
		struct is_const<Zero> {
			constexpr static bool value = true;
		};

	template <>
		struct is_const<Id> {
			constexpr static bool value = true;
		};

	template <class T>
		constexpr bool is_const_v = is_const<T>::value;

	template <class T, char...name>
		struct PH {
			static T* val;

			T getVal() const {
				return *val;
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return Zero();
				}

			template <class G, class O>
				void Optimize(const G& gradient, const O& optimizer) const {
				}

			operator T () const {
				return *val;
			}

			PH& operator=(T& rhs) {
				val = &rhs;
				return *this;
			}
			const PH& operator=(T& rhs) const {
				val = &rhs;
				return *this;
			}

			std::string getName() const {
				static char buffer[] = {name..., '\0'};
				return buffer;
			}
		};
	template <class T, char...name>
		T* PH<T, name...>::val = nullptr;

	template <class T = int, char...name>
		constexpr auto Placeholder(t_string<name...>) {
			return PH<T, name...>{};
		}

	template <class T>
		struct is_operation<PH<T>> {
			constexpr static bool value = true;
		};

}

#endif
