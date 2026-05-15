/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "header.h"
#include "unity.h"
#include "unity_fixture.h"
#include "unity_internals.h"

#define TEST_INSTANCES
#include "utils/self_assessment_utils.h"
#include "utils/self_assessment_utils1.h"


TEST_GROUP(Test3_Arrays);

//----------------------------------------------------------

INSTANCES_INIT

TEST_SETUP(Test3_Arrays)
{
    INSTANCES_SETUP
}

TEST_TEAR_DOWN(Test3_Arrays)
{
    INSTANCES_TEAR_DOWN
}

//----------------------------------------------------------

TEST(Test3_Arrays, testIntArrayWithinDeltaPointlessAndMessage)
{
    UNITY_INT16 expected[] = {5000, -4995, 5005};
    UNITY_INT16 actualBigDelta[] = {5200, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT16_ARRAY_WITHIN_MESSAGE(101, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END

}

TEST(Test3_Arrays, testNotEqualMemory2)
{
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 3);

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_MEMORY("fool", "food", 4);
    VERIFY_FAILS_END
}

TEST(Test3_Arrays, testEqualStrings)
{
    const char *testString = "foo";

    TEST_ASSERT_EQUAL_STRING(testString, testString);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("foo", "foo", "foo isn't foo");
    TEST_ASSERT_EQUAL_STRING("foo", testString);
    TEST_ASSERT_EQUAL_STRING(testString, "foo");
    TEST_ASSERT_EQUAL_STRING("", "");
}
