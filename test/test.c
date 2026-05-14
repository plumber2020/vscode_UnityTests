#include "test.h"

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_SUM_should_doZero);
    RUN_TEST(test_SUM_should_doTwo);
    return UNITY_END();
}