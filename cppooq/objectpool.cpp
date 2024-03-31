#include "objectpool.hpp"

#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

using namespace cppooq::objectpool::rt;

class UnsafeObjectPool : public ObjectPool {
public:
  explicit UnsafeObjectPool(std::size_t max_size, Produce produce)
      : max_size_{max_size}, consumption_count_{0}, produce_{produce} {
    pool_.reserve(max_size_);
  }

  ~UnsafeObjectPool() {
    if (not pool_.empty()) { pool_.clear(); }
  }

  std::size_t max_size() const noexcept final { return max_size_; }

  std::size_t consumption_count() const noexcept final {
    return consumption_count_;
  }

  std::unique_ptr<Poolable> Get() override {
    if (pool_.empty()) {
      if (consumption_count_ < max_size_) {
        consumption_count_++;
        return produce_();
      } else {
        throw std::length_error{"fail Get"};
      }
    } else {
      consumption_count_++;
      auto object = std::move(pool_.back());
      pool_.pop_back();
      return object;
    }
  }

  void Dispose(std::unique_ptr<Poolable> &&poolable) override {
    pool_.emplace_back(std::move(poolable));
    consumption_count_--;
  }

private:
  const std::size_t max_size_;
  std::size_t consumption_count_;
  std::vector<std::unique_ptr<Poolable>> pool_;
  Produce produce_;
};

class SafeObjectPool : public UnsafeObjectPool {
public:
  explicit SafeObjectPool(std::size_t max_size, Produce produce)
      : UnsafeObjectPool{max_size, produce} {}
  std::unique_ptr<Poolable> Get() override {
    const std::lock_guard<std::mutex> lock(pool_mutex_);
    return UnsafeObjectPool::Get();
  }

  void Dispose(std::unique_ptr<Poolable> &&poolable) override {
    const std::lock_guard<std::mutex> lock(pool_mutex_);
    UnsafeObjectPool::Dispose(
        std::forward<std::unique_ptr<Poolable>>(poolable));
  }

public:
  std::mutex pool_mutex_;
};

std::unique_ptr<ObjectPool> ObjectPool::MakeUnsafe(std::size_t max_size,
                                                   Produce produce) {
  return std::make_unique<UnsafeObjectPool>(max_size, produce);
}

std::unique_ptr<ObjectPool> ObjectPool::MakeSafe(std::size_t max_size,
                                                 Produce produce) {
  return std::make_unique<SafeObjectPool>(max_size, produce);
}
