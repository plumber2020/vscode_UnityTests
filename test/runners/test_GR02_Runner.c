/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(Test_Mult)
{
    RUN_TEST_CASE(Test_Mult, test_mult_positive_numbers);
    RUN_TEST_CASE(Test_Mult, test_mult_negative_numbers);
    RUN_TEST_CASE(Test_Mult, test_mult_mixed_signs);
    RUN_TEST_CASE(Test_Mult, test_mult_with_zero);
    RUN_TEST_CASE(Test_Mult, test_mult_large_numbers);
}


