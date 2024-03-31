#pragma once

#include <memory>
#include <numeric>
#include <string>

#include "cppooq.hpp"

namespace cppooq::hashtable::rt {

class Key {
  CPPOOQ_TRAIT(Key);

public:
  virtual std::size_t hash() const = 0;
  virtual bool operator==(const Key &k) const = 0;
  virtual std::unique_ptr<Key> Clone() const = 0;

  bool operator==(std::unique_ptr<Key> &k) const { return *this == *k; }
};

class Value {
  CPPOOQ_TRAIT(Value);
};

struct HashTableTraits {
  static std::size_t hash(const std::string &s) {
    return std::reduce(s.cbegin(), s.cend(), 0, [](std::size_t h, char c) {
      return h + static_cast<std::size_t>(c);
    });
  }
};

class HashTable {
  CPPOOQ_TRAIT(HashTable);

public:
  virtual std::size_t bucket_size() const noexcept = 0;

  virtual void Put(Key &key, std::unique_ptr<Value> &&value) = 0;
  virtual Value &Get(Key &key) = 0;

  template <class Value> void Put(Key &key, std::unique_ptr<Value> &value) {
    Put(key, std::move(value));
  }

  static std::unique_ptr<HashTable> Make(std::size_t bucket_size);
};

} // namespace cppooq::hashtable::rt
