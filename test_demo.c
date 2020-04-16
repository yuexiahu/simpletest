#include "simpletest.h"

static int sum(int a, int b)
{
    return a + b;
}

static double divide(double a, double b)
{
    return a / b;
}

static int step_ = 0;
static void next_step()
{
    step_++;
}

CASE(test_sum)
{
    EXPECT_EQ_INT(0, sum(0, 0));
    EXPECT_EQ_INT(0, sum(1, -1));
    EXPECT_EQ_INT(1, sum(0, 1));
    EXPECT_EQ_INT(-1, sum(-1, 0));
}

CASE(test_divide)
{
    EXPECT_EQ_DOUBLE(0.0, divide(0.0, 2.0));
    EXPECT_EQ_DOUBLE(1.0, divide(2.0, 2.0));
    EXPECT_EQ_DOUBLE(2.0, divide(4.0, 2.0));
}

CASE(test_concat)
{
    char string[256] = "";
    EXPECT_EQ_STR("", string);

    strcat(string, "abc");
    EXPECT_EQ_STR("abc", string);

    strcat(string, "def");
    EXPECT_EQ_STR("abcdef", string);
    EXPECT_EQ_STRN("abc", string, 3);
    EXPECT_EQ_MEM("abcdef", string, 6);
}

CASE(test_step)
{
    REQUIRE(step_ == 0);
    REQUIRE((next_step(), step_ == 1), "step_ is %d\n", step_);
    REQUIRE((next_step(), step_ == 2), "step_ is %d\n", step_);
    REQUIRE((next_step(), step_ == 3), "step_ is %d\n", step_);
    REQUIRE((next_step(), step_ == 4), "step_ is %d\n", step_);
    REQUIRE((next_step(), step_ == 5), "step_ is %d\n", step_);
}

UNIT(test_demo_entry,
        test_sum,
        test_divide,
        test_concat,
        test_step)
