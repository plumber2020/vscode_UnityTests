/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "header.h"
#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(Test1_Integers);

TEST_SETUP(Test1_Integers) {}

TEST_TEAR_DOWN(Test1_Integers) {}

//----------------------------------------------------------

TEST(Test1_Integers, iSumFunction_ShouldReturnZeroIfInputZero) 
{
    int expected = 0;
    int actual = iSum(0,0);
    TEST_ASSERT_EQUAL_INT(expected, actual);
}

TEST(Test1_Integers, iSumFunction_ShouldReturnTwoIfInputOnes) 
{
    int expected = 2;
    int actual = iSum(1,1);
    TEST_ASSERT_EQUAL_INT(expected, actual);
}

//----------------------------------------------------------

TEST(Test1_Integers, IgnoredTest)
{
    TEST_IGNORE_MESSAGE("This Test Was Ignored On Purpose");
}
