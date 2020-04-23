#include "simpletest.h"

void test_demo_entry();
UNIT(test_xxx)

SIMPLETEST_CONF(SIMPLETEST_ENABLE_UNIT_OUTPUT)
SIMPLETEST_LIST(main, test_demo_entry, test_xxx)
