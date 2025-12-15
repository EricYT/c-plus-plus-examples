#include "lru.h"

#include <string>
#include <iostream>

#include "gtest/gtest.h"

TEST(LRU, LRU) {
  LRU<uint32_t, std::string> lru(3);
  ASSERT_EQ(lru.size(), 0U);

  std::string val;
  bool ok = lru.Get(0, &val);
  ASSERT_FALSE(ok);

  lru.Put(0, "hello");
  ok = lru.Get(0, &val);
  ASSERT_TRUE(ok);
  ASSERT_EQ(val, "hello");

  lru.Put(0, "world");
  ok = lru.Get(0, &val);
  ASSERT_TRUE(ok);
  ASSERT_EQ(val, "world");

  lru.Put(1, "1");
  ok = lru.Get(1, &val);
  ASSERT_TRUE(ok);
  ASSERT_EQ(val, "1");

  lru.Put(2, "2");
  ok = lru.Get(2, &val);
  ASSERT_TRUE(ok);
  ASSERT_EQ(val, "2");

  lru.Put(3, "3");
  ok = lru.Get(3, &val);
  ASSERT_TRUE(ok);
  ASSERT_EQ(val, "3");

  ASSERT_EQ(lru.size(), 3U);

  // 0 already evict
  ok = lru.Get(0, &val);
  ASSERT_FALSE(ok);
}
