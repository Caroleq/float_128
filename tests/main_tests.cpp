#include <gtest/gtest.h>
#include "utilities_test.h"
#include "from_int_test.h"

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
