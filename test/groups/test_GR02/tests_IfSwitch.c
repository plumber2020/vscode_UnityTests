/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "if_switch.h"
#include "unity.h"
#include "unity_fixture.h"


#define EXECUTE_CYCLES(TIMEOUT)  \
    for (int i = 0; i < TIMEOUT; i++) { ifSwitchFunction();}


// Define the test group for this module
TEST_GROUP(Test_IfSwitch);

TEST_SETUP(Test_IfSwitch) {
    iResult = 0;
    eParameter = IDLE;
}

TEST_TEAR_DOWN(Test_IfSwitch) {}

// Tests ///////////////////////////////////////////////////////////////////

TEST(Test_IfSwitch, test_initial_state_should_not_increment_iResult) {
    EXECUTE_CYCLES(150)
    TEST_ASSERT_EQUAL(0, iResult);  // Still 0 because eParameter was IDLE
}

TEST(Test_IfSwitch, test_work_state_increments_iResult_after_100_calls) {
    eParameter = WORK;
    
    EXECUTE_CYCLES(IF_SWITCH_WORK_TIMEOUT);
    TEST_ASSERT_EQUAL(0, iResult);
    TEST_ASSERT_EQUAL(WORK, eParameter);

    ifSwitchFunction(); // 100-й вызов
    TEST_ASSERT_EQUAL(1, iResult);
    TEST_ASSERT_EQUAL(IDLE, eParameter);
}

TEST(Test_IfSwitch, test_multiple_work_cycles) {
    for (int cycle = 1; cycle < 3; cycle++) {
        eParameter = WORK;
        EXECUTE_CYCLES(IF_SWITCH_WORK_TIMEOUT);
        TEST_ASSERT_EQUAL(WORK, eParameter);
        ifSwitchFunction();
        TEST_ASSERT_EQUAL(cycle, iResult);
        TEST_ASSERT_EQUAL(IDLE, eParameter);
    }
}

TEST(Test_IfSwitch, test_state_change_to_idle_resets_counter) {
    eParameter = WORK;
    
    EXECUTE_CYCLES(50);
    TEST_ASSERT_EQUAL(0, iResult);

    // Change to IDLE (or any non-WORK) should reset counter
    eParameter = IDLE;
    ifSwitchFunction();  // Trigger reset logic

    // Now re-enter WORK — must start from zero
    eParameter = WORK;
    EXECUTE_CYCLES(IF_SWITCH_WORK_TIMEOUT);
    TEST_ASSERT_EQUAL(0, iResult);  // Not yet done

    ifSwitchFunction();
    TEST_ASSERT_EQUAL(1, iResult);
}

TEST(Test_IfSwitch, test_invalid_state_resets_counter) {
    eParameter = (Param)99;  // Invalid
    static int dummy = 0;
    for (int i = 0; i < 10; i++) {
        ifSwitchFunction();
    }
    TEST_ASSERT_EQUAL(0, iResult);  // No side effect
    // And if we later enter WORK, starts fresh
    eParameter = WORK;
    EXECUTE_CYCLES(IF_SWITCH_WORK_TIMEOUT);
    ifSwitchFunction();
    TEST_ASSERT_EQUAL(1, iResult);
}
