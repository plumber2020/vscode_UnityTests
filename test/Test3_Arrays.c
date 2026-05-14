#include "header.h"
#include "unity.h"
#include "unity_fixture.h"
#include "unity_internals.h"

#define TEST_INSTANCES
#include "self_assessment_utils.h"
#include "self_assessment_utils1.h"


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
    UNITY_INT expected[] = {5000, -4995, 5005};
    UNITY_INT actualBigDelta[] = {5101, -4896, 5055};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_ARRAY_WITHIN_MESSAGE(110, expected, actualBigDelta, 3, "Custom Message.");
    VERIFY_FAILS_END
}

