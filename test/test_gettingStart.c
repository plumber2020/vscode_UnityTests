#include "header.h"
#include "test.h"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_SUM_should_doZero(void) {
    int expected = 0;
    int actual = sum(0,0);
    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_SUM_should_doTwo(void) {
    int expected = 2;
    int actual = sum(1,1);
    TEST_ASSERT_EQUAL_INT(expected, actual);
}

