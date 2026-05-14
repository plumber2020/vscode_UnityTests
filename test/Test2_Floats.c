/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "header.h"
#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(Test2_Floats);

//----------------------------------------------------------

TEST_SETUP(Test2_Floats)
{
}

TEST_TEAR_DOWN(Test2_Floats)
{
}

//----------------------------------------------------------

#define EPSILON_FLOAT_EQUALITY   0.1f

TEST(Test2_Floats, fSumFunction_ShouldReturnZeroIfInputZero) 
{
    float expected = 0;
    float actual = fSum(0,0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON_FLOAT_EQUALITY, expected, actual);

}

TEST(Test2_Floats, fSumFunction_ShouldReturnThreeIfInputDifferentValues) 
{
    float expected = 3;
    float actual = fSum(0.5f,2.5f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON_FLOAT_EQUALITY, expected, actual);
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(2.95f, actual);

}
