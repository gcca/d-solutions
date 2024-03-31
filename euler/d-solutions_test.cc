#include "gtest/gtest.h"
#include <gtest/gtest.h>

#include "dsolutions.hpp"

class DSolutionsTest : public ::testing::Test {
protected:
  void TearDown() final { times.Show(); }
};

TEST_F(DSolutionsTest, Sol1) {
  EXPECT_EQ(dsol_1::sum_3_5_le(10), 23);
  EXPECT_EQ(dsol_1::sum_3_5_le_pro(10), 23);

  EXPECT_EQ(dsol_1::sum_3_5_le(1000), 233168);
  EXPECT_EQ(dsol_1::sum_3_5_le_pro(1000), 233168);

  EXPECT_EQ(TIMEIT(dsol_1::sum_3_5_le, 100000), 2333316668);
  EXPECT_EQ(TIMEIT(dsol_1::sum_3_5_le_pro, 100000), 2333316668);

  EXPECT_EQ(TIMEIT(dsol_1::sum_3_5_le, 1000000), 233333166668);
  EXPECT_EQ(TIMEIT(dsol_1::sum_3_5_le_pro, 1000000), 233333166668);
}

TEST_F(DSolutionsTest, Sol2) {
  EXPECT_EQ(TIMEIT(dsol_2::sumevenfib, 46368), 60696);
  EXPECT_EQ(TIMEIT(dsol_2::sumevenfib_step, 46368), 60696);

  EXPECT_EQ(TIMEIT(dsol_2::sumevenfib, 832040), 1089154);
  EXPECT_EQ(TIMEIT(dsol_2::sumevenfib_step, 832040), 1089154);

  EXPECT_EQ(TIMEIT(dsol_2::sumevenfib_step, 4000000), 4613732);
}
