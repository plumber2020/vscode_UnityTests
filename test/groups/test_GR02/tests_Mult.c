/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "header.h"
#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(Test1_Mult);

TEST_SETUP(Test1_Mult) {}

TEST_TEAR_DOWN(Test1_Mult) {}

//----------------------------------------------------------

TEST(Test1_Mult, test_mult_positive_numbers)
{
    TEST_ASSERT_EQUAL_INT(6, iMult(2, 3));
    TEST_ASSERT_EQUAL_INT(1, iMult(1, 1));
}

TEST(Test1_Mult, test_mult_negative_numbers)
{
    TEST_ASSERT_EQUAL_INT(6, iMult(-2, -3));
    TEST_ASSERT_EQUAL_INT(4, iMult(-2, -2));
}

TEST(Test1_Mult, test_mult_mixed_signs)

{
    TEST_ASSERT_EQUAL_INT(-6, iMult(2, -3));
    TEST_ASSERT_EQUAL_INT(-10, iMult(-5, 2));
}

TEST(Test1_Mult, test_mult_with_zero)
{
    TEST_ASSERT_EQUAL_INT(0, iMult(0, 5));
    TEST_ASSERT_EQUAL_INT(0, iMult(-3, 0));
}

TEST(Test1_Mult, test_mult_large_numbers)
{
    TEST_ASSERT_EQUAL_INT(1000000, iMult(1000, 1000));
    TEST_ASSERT_EQUAL_INT(-1000000, iMult(-1000, 1000));
}

