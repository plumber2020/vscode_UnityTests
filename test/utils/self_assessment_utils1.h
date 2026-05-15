#ifndef TEST_ASSESSMENT_UTILS_1
#define TEST_ASSESSMENT_UTILS_1


#define INSTANCES_INIT                                                                  \
    static int SetToOneToFailInTearDown;                                                \
    static int SetToOneMeanWeAlreadyCheckedThisGuy;                                     \

    
#define INSTANCES_SETUP                                                                 \
    SetToOneToFailInTearDown = 0;                                                       \
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;                                            \


#define INSTANCES_TEAR_DOWN                                                             \
    endPutcharSpy(); /* Stop suppressing test output */                                 \
    if (SetToOneToFailInTearDown == 1)                                                  \
    {                                                                                   \
        /* These will be skipped internally if already failed/ignored */                \
        TEST_FAIL_MESSAGE("<= Failed in tearDown");                                     \
        TEST_IGNORE_MESSAGE("<= Ignored in tearDown");                                  \
    }                                                                                   \
    if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))    \
    {                                                                                   \
        UnityPrint(": [[[[ Test Should Have Passed But Did Not ]]]]");                  \
        UNITY_OUTPUT_CHAR('\n');                                                        \
    }                                                                                   \


#endif // TEST_ASSESSMENT_UTILS_1
