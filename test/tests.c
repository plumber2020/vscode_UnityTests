/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "unity.h"
#include "tests.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testNotEqualInts);
    RUN_TEST(testNotLessOrEqualINT);
    return UNITY_END();
}


