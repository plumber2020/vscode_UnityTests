/* =========================================================================
    Unity - A Test Framework for C

    
========================================================================= */

#include "unity_fixture.h"


static void RunAllTests(void)
{
  RUN_TEST_GROUP(Test1_Integers);
  RUN_TEST_GROUP(Test2_Floats);
  RUN_TEST_GROUP(Test3_Arrays);
  
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
