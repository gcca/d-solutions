#include <algorithm>
#include <thread>

#include <gtest/gtest.h>

#include "objectpool.hpp"

using namespace cppooq::objectpool::rt;

class TestPoolable : public Poolable {};

using TestPoolableTraits = PoolableTraits<TestPoolable>;

TEST(UnsafeObjectPool, Create) {
  auto pool = ObjectPool::MakeUnsafe(2, TestPoolableTraits::simple_produce);

  EXPECT_EQ(pool->max_size(), 2);
}

TEST(UnsafeObjectPool, ProduceObject) {
  auto pool = ObjectPool::MakeUnsafe(2, TestPoolableTraits::simple_produce);

  auto obj_1 = pool->Get();

  EXPECT_FALSE(pool->fulfilled());

  auto obj_2 = pool->Get();

  EXPECT_TRUE(pool->fulfilled());

  std::size_t obj_2_id = reinterpret_cast<std::size_t>(obj_2.get());
  pool->Dispose(obj_2);

  EXPECT_FALSE(pool->fulfilled());

  auto obj_3 = pool->Get();
  std::size_t obj_3_id = reinterpret_cast<std::size_t>(obj_3.get());

  EXPECT_EQ(obj_3_id, obj_2_id);
}

TEST(SafeObjectPool, Create) {
  auto pool = ObjectPool::MakeSafe(2, TestPoolableTraits::simple_produce);

  EXPECT_EQ(pool->max_size(), 2);
}

TEST(SafeObjectPool, GetFromThreads) {
  const std::size_t test_threads_max_size = 200;

  auto pool = ObjectPool::MakeSafe(test_threads_max_size,
                                   TestPoolableTraits::simple_produce);

  std::vector<std::size_t> test_ids;
  std::vector<std::thread> test_threads;
  std::vector<std::unique_ptr<Poolable>> test_objs;

  test_objs.reserve(test_threads_max_size);
  test_ids.resize(test_threads_max_size);

  for (std::size_t i = 0; i < test_threads_max_size; i++) {
    test_threads.emplace_back(
        [&pool, &test_ids, &test_objs](const std::size_t i) {
          auto obj = pool->Get();
          test_ids.at(i) = reinterpret_cast<std::size_t>(obj.get());
          test_objs.emplace_back(std::move(obj));
        },
        i);
  }

  for (auto &test_thread : test_threads) {
    test_thread.join();
  }
  test_threads.clear();

  EXPECT_TRUE(pool->fulfilled());

  std::vector<std::size_t> obj_ids;
  obj_ids.reserve(test_threads_max_size);
  std::transform(
      test_objs.cbegin(), test_objs.cend(), std::back_inserter(obj_ids),
      [](auto &obj) { return reinterpret_cast<std::size_t>(obj.get()); });

  std::sort(test_ids.begin(), test_ids.end());
  std::sort(obj_ids.begin(), obj_ids.end());

  EXPECT_EQ(obj_ids, test_ids);

  for (std::size_t i = 0; i < test_threads_max_size; i++) {
    test_threads.emplace_back(
        [&pool, &test_objs](const std::size_t i) {
          pool->Dispose(test_objs.at(i));
        },
        i);
  }

  for (auto &test_thread : test_threads) {
    test_thread.join();
  }
  test_threads.clear();
  test_objs.clear();

  EXPECT_FALSE(pool->fulfilled());
}
