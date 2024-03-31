#include "lru.hpp"
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>

using namespace cppooq::lru::rt;

class LRUDerived : public LRU {
public:
  explicit LRUDerived(std::size_t max_size) : max_size_{max_size} {}

  Value &Get(const Key &key) final {
    std::size_t hash = key.hash();
    if (auto search = map_.find(hash); search != map_.end()) {
      auto &pair = search->second;

      counter_.erase(pair.first);
      counter_.push_front(hash);

      pair.first = counter_.begin();

      return *pair.second;
    }
    throw std::out_of_range{"no key"};
  }

  void Put(const Key &key, std::unique_ptr<Value> &&value) final {
    std::size_t hash = key.hash();
    if (auto search = map_.find(hash); search != map_.end()) {
      counter_.erase(search->second.first);

      counter_.push_front(hash);
      search->second = {counter_.begin(),
                        std::forward<std::unique_ptr<Value>>(value)};
    } else {
      if (counter_.size() >= max_size_) {
        std::size_t last_count = counter_.back();
        counter_.pop_back();
        map_.erase(last_count);
      }

      counter_.push_front(hash);
      map_[hash] = {counter_.begin(),
                    std::forward<std::unique_ptr<Value>>(value)};
    }
  }

private:
  const std::size_t max_size_;
  std::list<std::size_t> counter_;
  std::map<std::size_t,
           std::pair<std::list<std::size_t>::iterator, std::unique_ptr<Value>>>
      map_;
};

std::unique_ptr<LRU> LRU::Make(std::size_t max_size) {
  return std::make_unique<LRUDerived>(max_size);
}
