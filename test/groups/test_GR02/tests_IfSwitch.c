/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "if_switch.h"
#include "unity.h"
#include "unity_fixture.h"


// Define the test group for this module
TEST_GROUP(Test_IfSwitch);

TEST_SETUP(Test_IfSwitch) {
    iResult = 0;
    eParameter = IDLE;
}

TEST_TEAR_DOWN(Test_IfSwitch) {}

// Tests ///////////////////////////////////////////////////////////////////

TEST(Test_IfSwitch, test_initial_state_should_not_increment_iResult) {
    for (int i = 0; i < 150; i++) {
        ifSwitchFunction();
    }
    TEST_ASSERT_EQUAL(0, iResult);  // Still 0 because eParameter was IDLE
}

TEST(Test_IfSwitch, test_work_state_increments_iResult_after_100_calls) {
    eParameter = WORK;
    
    // Call 99 times: should not complete yet
    for (int i = 0; i < 99; i++) {
        ifSwitchFunction();
    }
    TEST_ASSERT_EQUAL(0, iResult);
    TEST_ASSERT_EQUAL(WORK, eParameter);

    // One more call → should trigger increment and reset
    ifSwitchFunction();
    TEST_ASSERT_EQUAL(1, iResult);
    TEST_ASSERT_EQUAL(IDLE, eParameter);
}

TEST(Test_IfSwitch, test_multiple_work_cycles) {
    for (int cycle = 0; cycle < 3; cycle++) {
        eParameter = WORK;
        for (int i = 0; i < 100; i++) {
            ifSwitchFunction();
        }
        TEST_ASSERT_EQUAL(cycle + 1, iResult);  // Should increment per full cycle
        TEST_ASSERT_EQUAL(IDLE, eParameter);
    }
}

TEST(Test_IfSwitch, test_state_change_to_idle_resets_counter) {
    eParameter = WORK;
    for (int i = 0; i < 50; i++) {
        ifSwitchFunction();
    }
    TEST_ASSERT_EQUAL(0, iResult);

    // Change to IDLE (or any non-WORK) should reset counter
    eParameter = IDLE;
    ifSwitchFunction();  // Trigger reset logic

    // Now re-enter WORK — must start from zero
    eParameter = WORK;
    for (int i = 0; i < 99; i++) {
        ifSwitchFunction();
    }
    TEST_ASSERT_EQUAL(0, iResult);  // Not yet done

    ifSwitchFunction();
    TEST_ASSERT_EQUAL(1, iResult);
}
