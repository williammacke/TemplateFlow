#ifndef TFMATH_H_
#define TFMATH_H_
#include <iostream>
#include <limits>
namespace TF {
	namespace math {
		constexpr int abs(int i) {
			return i<0?-i:i;
		}

		constexpr double powI(double base, int i) {
			double F{base};
			if (i == 0) {
				return 1.0;
			}
			else {
				for (int j = 1; j < abs(i); j++) {
					F *= base;
				}
				return i<0?1.0/F:F;
			}
		}

		constexpr unsigned long factorial(int i) {
			long F{1};
			if (i <= 1) {
				return 1;
			}
			else {
				for (int j = i; j > 1; j--) {
					F *= j;
				}
				return F;
			}
		}

		constexpr double exp(double y) {
			if (y < std::numeric_limits<double>::epsilon()) {
				return 0;
			}
			if (y > 10) {
				return powI(exp(y/2), 2);
			}
			double start{1.0};
			for (int i = 1; i < 20; i++) {
				auto term{pow(abs(y), i)/factorial(i)};
				start += term;
			}

			return y<0?1.0/start:start;
		}

		constexpr int digits(double num) {
			int count{0};
			int i{static_cast<int>(num)};
			while (i >0 ) {
				i /= 10;
				count++;
			}
			return count;
		}

		 constexpr double log(double x) {
			int d{digits(x)-1};
			double y{x/powI(10, d)};
			double start{1};
			for (int i = 0; i < 20; i++) {
				start += 2*(y-exp(start))/(y+exp(start));
			}
			return start + d*2.302;
		}

		 constexpr double pow(double base, double i) {
			return exp(log(base)*i);
		}
	}
}

#endif
