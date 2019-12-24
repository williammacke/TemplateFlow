#include <gtest/gtest.h>
#include "linalg.h"

TEST(test_linAlg, basic_test) 
{
	using namespace TF;
	constexpr Tensor<float, 2, 5> tensor = {{1,2,3,4,5}, {1,2,3,4,5}};
	constexpr Tensor<float, 2, 2> tensor2 = {};
	ASSERT_TRUE(tensor != tensor2);
	Tensor<float, 2, 5> tensor3 = {{1,2,3,4,5}, {1,2,3,4,5}};
	ASSERT_TRUE(tensor == tensor3);
	tensor3[0][0] = 2;
	ASSERT_TRUE(tensor != tensor3);
}

TEST(test_linAlg, dot_test) {
	using namespace TF;
	constexpr Vector<float, 3> v1 = {1,1,1};
	constexpr Vector<float, 3> v2 = {2,2,2};
	constexpr auto d = v1*v2;
	ASSERT_EQ(d, 6);
	Vector<float, 3> v3 = {3,3,3};
	auto e = v2*v3;
	ASSERT_EQ(e, 18);
}
