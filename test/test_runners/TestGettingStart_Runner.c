/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(Test1_Integers)
{
  RUN_TEST_CASE(Test1_Integers, iSumFunction_ShouldReturnZeroIfInputZero);
  RUN_TEST_CASE(Test1_Integers, iSumFunction_ShouldReturnTwoIfInputOnes);
  RUN_TEST_CASE(Test1_Integers, IgnoredTest);
}

TEST_GROUP_RUNNER(Test2_Floats)
{
  RUN_TEST_CASE(Test2_Floats, fSumFunction_ShouldReturnZeroIfInputZero);
  RUN_TEST_CASE(Test2_Floats, fSumFunction_ShouldReturnThreeIfInputDifferentValues);
}

TEST_GROUP_RUNNER(Test3_Arrays)
{
  RUN_TEST_CASE(Test3_Arrays, testIntArrayWithinDeltaPointlessAndMessage);
}
