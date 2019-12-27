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
	constexpr colVector<float, 3> v2 = {2,2,2};
	constexpr auto d = v1*v2;
	ASSERT_EQ(d, 6);
	rowVector<float, 3> v3 = {3,3,3};
	auto e = v3*v2;
	ASSERT_EQ(e, 18);
}

TEST(test_linAlg, cross_test) {
	using namespace TF; 
	constexpr Vector<float, 3> v1 = {1, 0, 0};
	constexpr rowVector<float, 3> v2 = {0,1,0};
	constexpr colVector<float, 3> v3 = {0,0,1};
	ASSERT_EQ(v1^v2, v3);
	ASSERT_EQ(v2^v3, v1);
	ASSERT_EQ(v3^v1, v2);
}

TEST(test_linAlg, matMul_test) {
	using namespace TF;
	constexpr Mat<float, 2, 2> m1 = {{1,1}, {1,1,}};
	constexpr Mat<float, 2, 2> m2 = {{1,1}, {1,1,}};
	constexpr auto m3 = m1*m2;
	Mat<float, 2, 2> m4 = {{2,2},{2,2}};
	ASSERT_EQ(m3, m4);
	constexpr Vector<float, 2> v = {1,1};
	constexpr auto res = m1*v;
	Vector<float, 2> v2 = {2,2};
	ASSERT_EQ(res, v2);
	constexpr rowVector<float, 2> t1 = {1,1};
	constexpr colVector<float, 2> t2 = {1, 1};
	constexpr auto t3 = t2*t1;
	ASSERT_EQ(t3, m1);
	constexpr Vector<float, 2> vec = {1,1};
	ASSERT_EQ(vec*t1, t3);
}

TEST(test_linAlg, trans_test) {
	using namespace TF;
	constexpr Mat<float, 2, 2> m1 = {{1, 2}, {1,2}};
	constexpr Mat<float, 2, 2> m2 = {{1,1}, {2,2}};
	constexpr auto m3 = trans(m1);
	ASSERT_EQ(m2, m3);
	constexpr Vector<float, 2> v1;
	constexpr rowVector<float, 2> v2;
	constexpr auto v3 = trans(v1);
	ASSERT_EQ(v2, v3);

}
