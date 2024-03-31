#include <gtest/gtest.h>
#include <memory>

#include "hashtable.hpp"

using namespace cppooq::hashtable::rt;

class TestKey : public Key {
public:
  TestKey(const std::string &s) : s_{s} {}

  bool operator==(const Key &k) const {
    return s_ == static_cast<const TestKey &>(k).s_;
  }

  std::unique_ptr<Key> Clone() const { return std::make_unique<TestKey>(s_); }

  std::size_t hash() const { return HashTableTraits::hash(s_); }

private:
  std::string s_;
};

class TestValue : public Value {
public:
  TestValue(const std::string &sv) : s{sv} {}
  std::string s;
};

TEST(HashTableTest, Append) {
  auto hashtable = HashTable::Make(10);

  EXPECT_EQ(hashtable->bucket_size(), 10);

  auto key_1 = TestKey{"key 1"};
  auto key_2 = TestKey{"key 2"};

  auto value_1 = std::make_unique<TestValue>("Value 1");
  auto value_2 = std::make_unique<TestValue>("Value 2");

  hashtable->Put(key_1, value_1);
  hashtable->Put(key_2, value_2);

  auto key_3 = TestKey{"key 1"};
  auto key_4 = TestKey{"key 4"};

  EXPECT_EQ(key_1, key_3);

  EXPECT_EQ(static_cast<TestValue &>(hashtable->Get(key_3)).s, "Value 1");
  EXPECT_EQ(static_cast<TestValue &>(hashtable->Get(key_2)).s, "Value 2");
}
