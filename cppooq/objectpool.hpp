#pragma once

#include <functional>
#include <memory>

#include "cppooq.hpp"

namespace cppooq::objectpool::rt {

class Poolable {
  CPPOOQ_TRAIT(Poolable);
};

using Produce = std::function<std::unique_ptr<Poolable>()>;

template <class T> struct PoolableTraits {
  static std::unique_ptr<T> simple_produce() { return std::make_unique<T>(); }
};

class ObjectPool {
  CPPOOQ_TRAIT(ObjectPool);

public:
  virtual std::size_t max_size() const noexcept = 0;
  virtual std::size_t consumption_count() const noexcept = 0;

  [[nodiscard]] virtual std::unique_ptr<Poolable> Get() = 0;
  virtual void Dispose(std::unique_ptr<Poolable> &&poolable) = 0;

  bool fulfilled() const noexcept { return consumption_count() == max_size(); }

  void Dispose(std::unique_ptr<Poolable> &poolable) {
    Dispose(std::move(poolable));
  }

  static std::unique_ptr<ObjectPool> MakeUnsafe(std::size_t max_size,
                                                Produce produce);

  static std::unique_ptr<ObjectPool> MakeSafe(std::size_t max_size,
                                              Produce produce);
};

} // namespace cppooq::objectpool::rt
