#include <sys/wait.h>
#include <iostream>

#include "tmp.h"

void Hi::SayHi() {
  std::cout << "Say Hi from Hi" << std::endl;
}

Hello::~Hello() {
  std::cout << "Hello deconstruction" << std::endl;
}

void Hello::SayHi() {
  std::cout << "Hello say hi" << std::endl;
}

MapList::~MapList() {
}

void MapList::Push(uint32_t key, uint32_t val) {
  if (kvs_.count(key) > 0) {
    return;
  }
  kvs_.emplace(key, val);
}

uint32_t PER_ROUND_BLOCK_ITEM_CNT = 4;

void SetPerRoundMaxItemCnt(uint32_t cnt) {
  PER_ROUND_BLOCK_ITEM_CNT = cnt;
}

void MapList::Run() {
  std::cout << "MapList::Run" << std::endl;
  static size_t curr_bucket_idx = 0;
  size_t bucket_cnt = kvs_.bucket_count();
  if (bucket_cnt == 0) {
    std::cout << "no buckets is here" << std::endl;
    return;
  }
  std::cout << "start bucket idx:" << curr_bucket_idx
    << " bucket cnt:" << bucket_cnt
    << std::endl;
  size_t round_start_idx = curr_bucket_idx;
  size_t cnt = 0;
  do {
    const auto bucket_idx = curr_bucket_idx % bucket_cnt;
    size_t bucket_size = kvs_.bucket_size(bucket_idx);
    if (bucket_size == 0) {
      std::cout << "BucketIndex:" << bucket_idx
        << " no items in it."
        << std::endl;
      ++curr_bucket_idx;
      continue;
    }
    if (cnt + bucket_size > PER_ROUND_BLOCK_ITEM_CNT && curr_bucket_idx != round_start_idx) {
      std::cout << "BucketIndex:" << bucket_idx
        << " over iterate max_cnt:" << PER_ROUND_BLOCK_ITEM_CNT
        << std::endl;
      break;
    }
    cnt += bucket_size;
    std::cout << "BucketIndex:" << bucket_idx
      << " size:" << bucket_size
      << " iterate_cnt:" << cnt
      << " max_cnt:" << PER_ROUND_BLOCK_ITEM_CNT
      << std::endl;
    for (auto iter = kvs_.cbegin(bucket_idx);
         iter != kvs_.cend(bucket_idx); ++iter) {
      std::cout << "Display BucketIndex:" << bucket_idx
        << " key:" << iter->first
        << " value:" << iter->second
        << std::endl;
    }
    ++curr_bucket_idx;
  } while ((curr_bucket_idx - round_start_idx < bucket_cnt) && cnt < PER_ROUND_BLOCK_ITEM_CNT);
  std::cout << "MapList::Run done" << std::endl;
}
