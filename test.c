#include "simpletest.h"

SIMPLETEST_DEF()

#define RUN(x) do { extern void x(); x(); } while(0)

int main()
{
    RUN(test_demo_entry);

    simpletest_output("all test finished: result is %s\n",
                      simpletest_result() ? "PASSED" : "FAILED");
    return 0;
}
