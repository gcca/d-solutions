#include "hashtable.hpp"
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace cppooq::hashtable::rt;

namespace {

struct Node {
  Node(Key &k, std::unique_ptr<Value> &&v, Node *n)
      : key{k.Clone()}, value{std::move(v)}, next{n} {}

  std::unique_ptr<Key> key;
  std::unique_ptr<Value> value;
  Node *next;
};

class HashTableDerived : public HashTable {
public:
  explicit HashTableDerived(std::size_t bucket_size)
      : bucket_size_{bucket_size} {
    buckets_.resize(bucket_size);
  }

  std::size_t bucket_size() const noexcept final { return bucket_size_; }

  void Put(Key &key, std::unique_ptr<Value> &&value) final {
    std::size_t i = key.hash() % bucket_size_;
    auto new_node = std::make_unique<Node>(
        key, std::forward<std::unique_ptr<Value>>(value), buckets_[i].get());
    buckets_[i] = std::move(new_node);
  };

  Value &Get(Key &key) final {
    std::size_t i = key.hash() % bucket_size_;
    Node *node = buckets_[i].get();

    while (node != nullptr) {
      if (*node->key == key) { return *node->value; }
      node = node->next;
    }

    throw std::out_of_range{"no key"};
  }

private:
  const std::size_t bucket_size_;
  std::vector<std::unique_ptr<Node>> buckets_;
};

} // namespace

std::unique_ptr<HashTable> HashTable::Make(std::size_t bucket_size) {
  return std::make_unique<HashTableDerived>(bucket_size);
}
