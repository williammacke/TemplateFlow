#ifndef OPERATION_H_
#define OPERATION_H_
#include "Variable.h"
#include <memory>
#include <type_traits>
#include <cmath>
#include <unordered_map>
#include <string>
#include "math.h"
#include "include/gcem.hpp"
#include "linalg.h"

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









	template <class T, class E>
		struct Addition;

	template <class T, class E>
		struct is_operation<Addition<T, E>> {
			static constexpr bool value = true;
		};

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto operator+(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			if constexpr(std::is_same_v<decltype(lhs2), Zero>) {
				return rhs2;
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Zero>) {
				return lhs2;
			}
			else if constexpr((is_const_v<T>) && (is_const_v<E>)) {
				return Value<decltype(lhs2.val+rhs2.val)>(lhs2.val+rhs2.val); 
			}
			else {
				return Addition<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
			}
		}

	template <class T, class E>
		struct Addition {
			const T lhs;
			const E rhs;

			constexpr Addition(const T& v1, const E& v2) : lhs(v1), rhs(v2) { }

			decltype(lhs.getVal()+rhs.getVal()) getVal() const {
				return lhs.getVal() + rhs.getVal();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) const {
					return lhs.getDerivative(var) + rhs.getDerivative(var);
				}

			template <class G, class O>
				void Optimize(const G& gradient, const O& optimizer) const {
					lhs.optimize(gradient, optimizer);
					rhs.optimize(gradient, optimizer);
				}
			operator decltype(lhs.getVal()+rhs.getVal())() const {
				return lhs.getVal() + rhs.getVal();
			}

			std::string getName() const {
				return "(" + lhs.getName() + " + " + rhs.getName() + ")";
			}

			static constexpr auto getVars() {
				return std::tuple_cat(lhs.getVars(), rhs.getVars());
			}
		};

	template <class T, class E>
		struct Subtraction;

	template <class T, class E>
		struct is_operation<Subtraction<T, E>> {
			constexpr static bool value = true;
		};


	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto operator-(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			if constexpr(std::is_same_v<decltype(lhs2), Zero>) {
				return -1*rhs2;
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Zero>) {
				return lhs2;
			}
			else if constexpr((is_const_v<T>) && (is_const_v<E>)) {
				return Value<decltype(lhs2.val-rhs2.val)>(lhs2.val-rhs2.val); 
			}
			else {
				return Subtraction<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
			}
		}

	template <class T, class E>
		struct Subtraction {
			const T lhs;
			const E rhs;
			constexpr Subtraction(const T& v1, const E& v2) : lhs(v1), rhs(v2) { }

			decltype(lhs.getVal() - rhs.getVal()) getVal() const {
				return lhs.getVal() - rhs.getVal();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) const {
					return lhs.getDerivative(var) - rhs.getDerivative(var);
				}

			operator decltype(lhs.getVal() - rhs.getVal()) () const {
				return lhs.getVal() - rhs.getVal();
			}

			std::string getName() const {
				return "(" + lhs.getName() + " - " + rhs.getName() + ")";
			}

			static constexpr auto getVars() {
				return std::tuple_cat(lhs.getVars(), rhs.getVars());
			}
		};

	template <class T, class E>
		struct Multiplication;

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto operator*(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			if constexpr(std::is_same_v<decltype(lhs2), Zero> || std::is_same_v<decltype(rhs2), Zero>) {
				return Zero();
			}
			else if constexpr(std::is_same_v<decltype(lhs2), Id>) {
				return rhs2;
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Id>) {
				return lhs2;
			}
			else if constexpr(is_const_v<T> && is_const_v<E>) {
				return Value<decltype(lhs2.getVal()*rhs2.getVal())>(lhs2.getVal()*rhs2.getVal());
			}
			else {
				return Multiplication<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
			}
		}

	template <class T, class E>
		struct Multiplication {
			T lhs;
			E rhs;
			constexpr Multiplication(const T& lhs, const E& rhs) : lhs(lhs), rhs(rhs) { }

			auto getVal() const {
				return lhs.getVal() * rhs.getVal();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return lhs.getDerivative(var)*rhs + lhs*rhs.getDerivative(var);
				}
			operator decltype(lhs.getVal()*rhs.getVal()) () const {
				return lhs.getVal() * rhs.getVal();
			}

			std::string getName() const {
				return "(" + lhs.getName() + " * " + rhs.getName() + ")";
			}

			static constexpr auto getVars() {
				return std::tuple_cat(lhs.getVars(), rhs.getVars());
			}


		};

	template <class T, class E>
		struct Division;

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto operator/(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			if constexpr(std::is_same_v<decltype(lhs2), Zero>) {
				return Zero();
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Id) {
				return lhs2;
			}
			else if constexpr(is_const_v<T> && is_const_v<E>) {
				return Value<decltype(lhs2.getVal()/rhs2.getVal())>(lhs2.getVal()/rhs2.getVal());
			}
			else {
				return Division<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
			}
		}

	template <class T, class E>
		struct Division {
			T lhs;
			E rhs;
			constexpr Division(const T& lhs, const E& rhs) : lhs(lhs), rhs(rhs) { }

			decltype(lhs.getVal() / rhs.getVal()) getVal() const {
				return lhs.getVal() / rhs.getVal();
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return (lhs.getDerivative(var)*rhs - lhs*rhs.getDerivative(var))/(rhs*rhs);
				}

			operator decltype(lhs.getVal() / rhs.getVal()) () const {
				return getVal();
			}

			std::string getName() const {
				return "(" + lhs.getName() + " / " + rhs.getName() + ")";
			}

			static constexpr auto getVars() {
				return std::tuple_cat(lhs.getVars(), rhs.getVars());
			}
		};

	template <class T, class E>
		struct Pow;

	template <class T>
		struct ln;
		

	template <class T, class E, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto operator^(const T& lhs, const E& rhs) {
			auto lhs2 = value(lhs);
			auto rhs2 = value(rhs);
			if constexpr(std::is_same_v<decltype(lhs2), Zero>) {
				return Zero();
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Zero>) {
				return Id();
			}
			else if constexpr(std::is_same_v<decltype(lhs2), Id>) {
				return Id();
			}
			else if constexpr(std::is_same_v<decltype(rhs2), Id>) {
				return lhs2;
			}
			else if constexpr(is_const_v<decltype(lhs2)> && is_const_v<decltype(rhs2)>) {
				return Value<decltype(gcem::pow(lhs2.getVal(), rhs2.getVal()))>(gcem::pow(lhs2.getVal(), rhs2.getVal()));
			}
			else {
				return Pow<decltype(lhs2), decltype(rhs2)>(lhs2, rhs2);
			}
		}

	template <class T, typename=std::enable_if_t<is_operation_v<T> || is_operation_v<E>>>
		constexpr auto ln(const T& lhs) {
			auto lhs2 = value(lhs);
			if constexpr(std::is_same_v<decltype(lhs), Id>) {
				return Zero();
			}
			else if constexpr(is_const_v<decltype(lhs2)>) {
				return Value<decltype(gcem::log(lhs2.getVal()))>(gcem::log(lhs2.getVal()));
			}
			else {
				return ln<decltype(lhs2)>(lhs2);
			}
		}

	template <class T, class E>
		struct Pow {
			const T lhs;
			const E rhs;
			constexpr Pow(const T& lhs, const E& rhs) : lhs(lhs), rhs(rhs) { }

			auto getVal() const {
				return pow(lhs.getVal(), rhs.getVal());
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return (lhs^rhs)*(rhs.getDerivative(var)*ln(lhs) + lhs.getDerivative(var)*rhs/lhs);
					
				}

			operator decltype(getVal()) () const {
				return getVal();
			}

			std::string getName() const {
				return "(" + lhs.getName() + " ^ " + rhs.getName() + ")";
			}

			constexpr static auto getVars() {
				return std::tuple_cat(lhs.getVars(), rhs.getVars());
			}
		};

	template <class T>
		struct ln {
			const T lhs;
			constexpr ln(const T& lhs) : lhs(lhs) { }

			auto getVal() const {
				return log(lhs.getVal());
			}

			template <class V>
				constexpr auto getDerivative(const V& var) {
					return (1.0/lhs)*lhs.getDerivative(var);
				}

			operator decltype(getVal()) () const {
				return getVal();
			}

			std::string getname() const {
				return "ln(" + lhs.getName() + ")";
			}

			constexpr static auto getVars() {
				return lhs.getVars();
			}
		};


}

#endif
