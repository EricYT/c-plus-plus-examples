#ifndef __COMMON_LRU__
#define __COMMON_LRU__

#include <iostream>
#include <list>
#include <unordered_map>

template <typename KeyType, typename ValueType>
class LRU {
 public:
  explicit LRU(uint64_t cap) : cap_(cap) {}

  ~LRU() {
    lru_.clear();
    kvs_.clear();
  }

  bool Get(const KeyType& key, ValueType* val) {
    const auto& iter = kvs_.find(key);
    if (iter == kvs_.end()) {
      return false;
    }
    LRUItemIter item_iter = iter->second;
    *val = item_iter->value;
    lru_.splice(lru_.begin(), lru_, item_iter);
    return true;
  }

  void Put(const KeyType& key, const ValueType& val) {
    const auto& iter = kvs_.find(key);
    if (iter != kvs_.end()) {
      LRUItemIter item_iter = iter->second;
      (*item_iter).value = val;
      lru_.splice(lru_.begin(), lru_, item_iter);
      return;
    }
    auto item_iter = lru_.emplace_front(key, val);
    kvs_.emplace(key, lru_.begin());

    if (kvs_.size() > cap_) {
      auto item_iter = lru_.back();
      lru_.pop_back();
      kvs_.erase(item_iter.key);
    }
  }

  uint64_t size() const { return lru_.size(); }

  constexpr uint64_t cap() const { return cap_; }

 public:
  struct LRUItem {
    KeyType key;
    ValueType value;

    explicit LRUItem(const KeyType& key, const ValueType& val)
        : key(key), value(val) {}
  };
  using LRUItemIter = typename std::list<LRUItem>::iterator;
  using LRUItemConstIter = typename std::list<LRUItem>::const_iterator;

 private:
  const uint64_t cap_;

  std::list<LRUItem> lru_;
  std::unordered_map<KeyType, LRUItemIter> kvs_;
};

#endif  // __COMMON_LRU__
