#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>

#include "lru.hpp"

using namespace cppooq::lru::rt;

class TestValue : public Value {
public:
  TestValue(const std::string &&v) : s{v} {}

  std::string s;
};

TEST(LRU, Limit) {
  auto key_1 = TypedKey<std::string>("key 1");
  auto key_2 = TypedKey<std::string>("key 2");
  auto key_3 = TypedKey<std::string>("key 3");
  auto key_4 = TypedKey<std::string>("key 4");

  auto value_1 = std::make_unique<TestValue>("Value 1");
  auto value_2 = std::make_unique<TestValue>("Value 2");
  auto value_3 = std::make_unique<TestValue>("Value 3");
  auto value_4 = std::make_unique<TestValue>("Value 4");

  auto lru = LRU::Make(2);

  lru->Put(key_1, value_1);
  lru->Put(key_2, value_2);

  EXPECT_EQ(static_cast<TestValue &>(lru->Get(key_1)).s,
            std::string{"Value 1"});

  lru->Put(key_3, value_3);

  EXPECT_THROW(lru->Get(key_2), std::out_of_range);

  lru->Put(key_4, value_4);

  EXPECT_THROW(lru->Get(key_1), std::out_of_range);
  EXPECT_EQ(static_cast<TestValue &>(lru->Get(key_3)).s,
            std::string{"Value 3"});
  EXPECT_EQ(static_cast<TestValue &>(lru->Get(key_4)).s,
            std::string{"Value 4"});
}
