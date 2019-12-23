#ifndef OPERATION_H_
#define OPERATION_H_
#include "Variable.h"
#include <memory>
#include <type_traits>
#include <cmath>
#include <unordered_map>
#include <string>
#include "math.h"
#include "gcem/include/gcem.hpp"

namespace TF {

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator==(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return lhs2.getVal() == rhs2.getVal();
	}

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator!=(const T& lhs, const E& rhs) {
		return !(lhs == rhs);
	}

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator<(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return lhs.getVal() < rhs.getVal();
	}

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator<=(const T& lhs, const E& rhs) {
		return lhs == rhs || lhs < rhs;
	}

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator>(const T& lhs, const E& rhs) {
		return !(lhs <= rhs);
	}

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
	constexpr bool operator>=(const T& lhs, const E& rhs) {
		return !(lhs < rhs);
	}

	template <class T, class E, typename=std::enable_if_t<(is_value_v<T> && (is_value_v<E> || !is_operation_v<E>)) || 
		(is_value_v<E> && (is_value_v<T> || !is_operation_v<T>))>>
	constexpr auto operator+(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return Value<decltype(lhs2.val+rhs2.val)>(lhs2.val+rhs2.val); 
	}

	template <class T, class E, typename=std::enable_if_t<(is_value_v<T> && (is_value_v<E> || !is_operation_v<E>)) || 
		(is_value_v<E> && (is_value_v<T> || !is_operation_v<T>))>>
	constexpr auto operator-(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return Value<decltype(lhs2.val-rhs2.val)>(lhs2.val-rhs2.val); 
	}

	template <class T, class E, typename=std::enable_if_t<(is_value_v<T> && (is_value_v<E> || !is_operation_v<E>)) || 
		(is_value_v<E> && (is_value_v<T> || !is_operation_v<T>))>>
	constexpr auto operator*(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return Value<decltype(lhs2.getVal()*rhs2.getVal())>(lhs2.getVal()*rhs2.getVal()); 
	}

	template <class T, class E, typename=std::enable_if_t<(is_value_v<T> && (is_value_v<E> || !is_operation_v<E>)) || 
		(is_value_v<E> && (is_value_v<T> || !is_operation_v<T>))>>
	constexpr auto operator/(const T& lhs, const E& rhs) {
		auto lhs2 = value(lhs);
		auto rhs2 = value(rhs);
		return Value<decltype(lhs2.getVal()/rhs2.getVal())>(lhs2.getVal()/rhs2.getVal()); 
	}

	template <class T, class E, typename=std::enable_if_t<(is_value_v<T> && (is_value_v<E> || !is_operation_v<E>)) ||
		(is_value_v<E> && (is_value_v<T> || !is_operation_v<T>))>>
		constexpr auto operator^(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			return Value<decltype(gcem::pow(lhs2.getVal(), rhs2.getVal()))>(gcem::pow(lhs2.getVal(), rhs2.getVal()));
		}




	template <class T, class E>
		struct Addition;

	template <class T, class E>
		struct is_operation<Addition<T, E>> {
			static constexpr bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<(is_operation_v<T> && !is_value_v<T>) || (is_operation_v<E> && !is_value_v<T>)>, int=0>
		constexpr auto operator+(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			return Addition<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
		}

	template <class T, class E>
		struct Addition {
			T lhs;
			E rhs;

			constexpr Addition(const T& v1, const E& v2) : lhs(v1), rhs(v2) { }

			decltype(lhs.getVal()+rhs.getVal()) getVal() const {
				return lhs.getVal() + rhs.getVal();
			}

			void initVars() {
				lhs.initVars();
				rhs.initVars();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return lhs.getDerivative(var) + rhs.getDerivative(var);
				}
			operator decltype(lhs.getVal()+rhs.getVal())() const {
				return lhs.getVal() + rhs.getVal();
			}
		};

	template <class T, class E>
		struct Subtraction;

	template <class T, class E>
		struct is_operation<Subtraction<T, E>> {
			constexpr static bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<(is_operation_v<T> && !is_value_v<T>) || (is_operation_v<E> && !is_value_v<E>)>, int=0>
		constexpr auto operator-(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			return Subtraction<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
		}

	template <class T, class E>
		struct Subtraction {
			T lhs;
			E rhs;
			constexpr Subtraction(const T& v1, const E& v2) : lhs(v1), rhs(v2) { }

			decltype(lhs.getVal() - rhs.getVal()) getVal() const {
				return lhs.getVal() - rhs.getVal();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return lhs.getDerivative(var) - rhs.getDerivative(var);
				}

			operator decltype(lhs.getVal() - rhs.getVal()) () {
				return lhs.getVal() - rhs.getVal();
			}
		};


}

#endif
