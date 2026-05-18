/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "default_test.h"
#include "unity.h"


void testNotEqualInts(void)
{
  uint16_t actual = 1000;
  uint16_t expected = 1000;
  TEST_ASSERT_EQUAL_INT(expected, actual);
}
void testNotLessOrEqualINT(void)
{
  uint16_t actual = 1000;
  uint16_t threshold = 1000;
  TEST_ASSERT_LESS_OR_EQUAL_INT(threshold, actual);
}
