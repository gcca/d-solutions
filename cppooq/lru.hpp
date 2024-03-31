#pragma once

#include <functional>
#include <memory>

#include "cppooq.hpp"

namespace cppooq::lru::rt {

class Key {
  CPPOOQ_TRAIT(Key);

public:
  virtual std::size_t hash() const = 0;
};

template <class T> class TypedKey : public Key {
public:
  explicit TypedKey(T t) : t_{t} {}

  std::size_t hash() const final { return std::hash<T>{}(t_); }

private:
  T t_;
};

class Value {
  CPPOOQ_TRAIT(Value);
};

class LRU {
  CPPOOQ_TRAIT(LRU);

public:
  virtual Value &Get(const Key &key) = 0;
  virtual void Put(const Key &key, std::unique_ptr<Value> &&value) = 0;

  void Put(const Key &key, std::unique_ptr<Value> &value) {
    Put(key, std::move(value));
  }

  template <class T> void Put(const Key &key, std::unique_ptr<T> &value) {
    Put(key, std::move(value));
  }

  static std::unique_ptr<LRU> Make(std::size_t max_size);
};

} // namespace cppooq::lru::rt
