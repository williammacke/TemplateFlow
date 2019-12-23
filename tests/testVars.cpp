#include <gtest/gtest.h>
#include "Variable.h"

TEST(test_vars, value_test)
{
	using namespace TF;
	constexpr auto val = value(5);
	ASSERT_EQ(val, 5);
	ASSERT_TRUE(val != 7);
	ASSERT_TRUE(val < 7);
	ASSERT_TRUE(val <= 7 && val <= 5);
	ASSERT_TRUE(val > 4);
	ASSERT_TRUE(val >= 4 && val >= 5);
}

TEST(test_vars, var_test)
{
	using namespace TF;
	constexpr auto var = Variable("Var"_tstr);
	ASSERT_EQ(var, 0);
	var = 5;
	ASSERT_EQ(var, 5);
	ASSERT_TRUE(var != 0);
	ASSERT_TRUE(var < 7);
	ASSERT_TRUE(var <= 7 && var <= 5);
	ASSERT_TRUE(var > 4);
	ASSERT_TRUE(var >= 4 && var >= 5);
}

TEST(test_vars, placeholder_test)
{
	using namespace TF;
	constexpr auto ph = Placeholder("PH"_tstr);
	int a = 5;
	ph = a;
	ASSERT_EQ(ph, a);
	ASSERT_EQ(ph, 5);
	ASSERT_TRUE(ph != 7);
	ASSERT_TRUE(ph < 7);
	ASSERT_TRUE(ph <= 7 && ph <= 5);
	ASSERT_TRUE(ph > 4);
	ASSERT_TRUE(ph >= 4 && ph >= 5);
}

TEST(test_vars, all_test)
{
	using namespace TF;
	constexpr auto val = value(5);
	constexpr auto var = Variable("var"_tstr);
	constexpr auto ph = Placeholder("ph"_tstr);
	ASSERT_TRUE(val != var && val != ph);
	var = 5;
	ASSERT_TRUE(val == var && val != ph);
	int a = 5;
	ph = a;
	ASSERT_TRUE(val == var && val == ph);
	a = 6;
	ASSERT_TRUE(val <= var && val < ph);
	ASSERT_TRUE(var >= val && ph > val);
}
