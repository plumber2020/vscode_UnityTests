/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(Test1_Mult)
{
    RUN_TEST_CASE(Test1_Mult, test_mult_positive_numbers);
    RUN_TEST_CASE(Test1_Mult, test_mult_negative_numbers);
    RUN_TEST_CASE(Test1_Mult, test_mult_mixed_signs);
    RUN_TEST_CASE(Test1_Mult, test_mult_with_zero);
    RUN_TEST_CASE(Test1_Mult, test_mult_large_numbers);
}

TEST_GROUP_RUNNER(Test2_NewFunctions)
{
    RUN_TEST_CASE(Test_IfSwitch, test_initial_state_should_not_increment_iResult);
    RUN_TEST_CASE(Test_IfSwitch, test_work_state_increments_iResult_after_100_calls);
    RUN_TEST_CASE(Test_IfSwitch, test_multiple_work_cycles);
    RUN_TEST_CASE(Test_IfSwitch, test_state_change_to_idle_resets_counter);
    
} 



