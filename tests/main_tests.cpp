#include <gtest/gtest.h>
#include "addition_test.h"
#include "utilities_test.h"
#include "multiplication_test.h"
#include "from_int_test.h"
#include "equality_test.h"
#include "substraction_test.h"
#include "from_double_test.h"
#include "comparison_test.h"
#include "vector_test.h"

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
