#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>

template <size_t N>
class MetaDataLayout {
 public:
  explicit MetaDataLayout() {}
  ~MetaDataLayout() {}

  size_t FindContinuousFreeSpace(size_t cnt) {
    return FindContinuousFreeSpace(
        cnt, [&](size_t n) -> size_t { return FindFirstZero(space_, n); },
        [&](size_t n) -> size_t { return FindFirstOne(space_, n); });
  }

  size_t FindContinuousFreeSpaceV1(size_t cnt) {
    return FindContinuousFreeSpace(
        cnt, [&](size_t n) -> size_t { return FindFirstZeroV1(space_, n); },
        [&](size_t n) -> size_t { return FindFirstOneV1(space_, n); });
  }

  void MarkSpaceUsed(const std::vector<size_t>& used);

  void MarkSpaceFreed(const std::vector<size_t>& freed);

 public:
  void DEBUG_reset_space(const std::bitset<N>& space) { space_ = space; }

 private:
  using FuncFindFirstX = std::function<size_t(size_t)>;

  size_t FindContinuousFreeSpace(size_t cnt, FuncFindFirstX find_zero,
                                 FuncFindFirstX find_one);

  static size_t FindFirstZero(const std::bitset<N>& space, size_t from);
  static size_t FindFirstOne(const std::bitset<N>& space, size_t from);

  size_t FindFirstZeroV1(const std::bitset<N>& space, size_t from);
  size_t FindFirstOneV1(const std::bitset<N>& space, size_t from);

 private:
  // NOTICE: std::bitset<N> index from right to left
  // "0011111" index 0 is 1 not 0
  std::bitset<N> space_;
  // boost::dynamic_bitset<>
};

// DUMP WAY
template <size_t N>
size_t MetaDataLayout<N>::FindFirstZero(const std::bitset<N>& space,
                                        size_t from) {
  for (; from < N; ++from) {
    if (!space.test(from)) {
      return from;
    }
  }
  return N;
}

template <size_t N>
size_t MetaDataLayout<N>::FindFirstOne(const std::bitset<N>& space,
                                       size_t from) {
  for (; from < N; ++from) {
    if (space.test(from)) {
      return from;
    }
  }
  return N;
}

template <size_t N>
size_t MetaDataLayout<N>::FindContinuousFreeSpace(
    size_t cnt, FuncFindFirstX func_find_zero, FuncFindFirstX func_find_one) {
  if (cnt == 0) return N;

  if (N - space_.count() < cnt) return N;

  size_t curr = 0;
  while (curr <= N - cnt) {
    size_t first_zero = func_find_zero(curr);
    if (first_zero == N) {
      return N;
    }

    size_t first_one = func_find_one(first_zero + 1);
    if (first_one - first_zero >= cnt) {
      return first_zero;
    }
    curr = first_one + 1;
  }
  return N;
}

// ANOTHER
bool FindFirstZeroUInt64(const uint64_t v, size_t* p) {
  if (v == UINT64_MAX) return false;
  *p = __builtin_ctzll(~v);
  return true;
}

bool FindFirstOneUInt64(const uint64_t v, size_t* p) {
  if (v == 0) return false;
  *p = __builtin_ctzll(v);
  return true;
}

const static size_t CHUNK_SIZE_64_BITS = 64;

template <size_t N>
size_t MetaDataLayout<N>::FindFirstZeroV1(const std::bitset<N>& _space,
                                          size_t from) {
  std::bitset<N> MARSK{UINT64_MAX};
  std::bitset<N> space = _space;
  space >>= from;
  size_t n = N - from;
  size_t total_chunk = (n + CHUNK_SIZE_64_BITS - 1) / CHUNK_SIZE_64_BITS;
  std::bitset<N> chunk;
  for (size_t chunk_index = 0; chunk_index < total_chunk; ++chunk_index) {
    chunk = space & MARSK;
    size_t pos = 0;
    bool ok = FindFirstZeroUInt64(chunk.to_ullong(), &pos);
    if (ok) {
      return pos + chunk_index * CHUNK_SIZE_64_BITS + from;
    }
    space >>= 64;
  }
  return N;
}

template <size_t N>
size_t MetaDataLayout<N>::FindFirstOneV1(const std::bitset<N>& _space,
                                         size_t from) {
  std::bitset<N> MARSK{UINT64_MAX};
  std::bitset<N> space = _space;
  space >>= from;
  size_t n = N - from;
  size_t total_chunk = (n + CHUNK_SIZE_64_BITS - 1) / CHUNK_SIZE_64_BITS;
  std::bitset<N> chunk;
  for (size_t chunk_index = 0; chunk_index < total_chunk; ++chunk_index) {
    chunk = space & MARSK;
    size_t pos = 0;
    bool ok = FindFirstOneUInt64(chunk.to_ullong(), &pos);
    if (ok) {
      return pos + chunk_index * CHUNK_SIZE_64_BITS + from;
    }
    space >>= 64;
  }
  return N;
}

template <size_t N>
void MetaDataLayout<N>::MarkSpaceUsed(const std::vector<size_t>& used) {
  for (auto& pos : used) {
    space_.set(pos, true);
  }
}

template <size_t N>
void MetaDataLayout<N>::MarkSpaceFreed(const std::vector<size_t>& freed) {
  for (auto& pos : freed) {
    space_.set(pos, false);
  }
}

void TestMetaDataLayout() {
  static const size_t N = 10;
  MetaDataLayout<N> meta;

  // test 0
  std::bitset<N> tmp{"0000000000"};
  meta.DEBUG_reset_space(tmp);
  auto index = meta.FindContinuousFreeSpace(10);
  assert(index == 0);

  index = meta.FindContinuousFreeSpace(11);
  assert(index == N);

  // test 1
  std::bitset<N> tmp1{"1100100010"};
  meta.DEBUG_reset_space(tmp1);
  index = meta.FindContinuousFreeSpace(10);
  assert(index == N);

  index = meta.FindContinuousFreeSpace(3);
  assert(index == 2u);

  // 0001101010
  meta.MarkSpaceUsed({3, 6});
  meta.MarkSpaceFreed({8, 9});

  index = meta.FindContinuousFreeSpace(3);
  assert(index == 7u);

  index = meta.FindContinuousFreeSpace(4);
  assert(index == N);
}

void TestMetaDataLayoutV1() {
  static const size_t N = 140;
  MetaDataLayout<N> meta;

  // test 0
  std::string one_65;
  one_65.assign(65, '1');
  std::bitset<N> tmp{"000000110000000" + one_65};
  meta.DEBUG_reset_space(tmp);

  size_t index = meta.FindContinuousFreeSpaceV1(8);
  assert(index == 74);

  index = meta.FindContinuousFreeSpaceV1(4);
  assert(index == 65);

  // test 1
  std::bitset<N> tmp1{std::string(140, '1')};
  meta.DEBUG_reset_space(tmp1);
  index = meta.FindContinuousFreeSpaceV1(1);
  assert(index == N);

  meta.MarkSpaceFreed({138, 137});
  index = meta.FindContinuousFreeSpaceV1(2);
  assert(index == 137);
  index = meta.FindContinuousFreeSpaceV1(3);
  assert(index == N);
  meta.MarkSpaceFreed({139});
  index = meta.FindContinuousFreeSpaceV1(3);
  assert(index == 137);
}

int main() {
  TestMetaDataLayout();
  TestMetaDataLayoutV1();
  return 0;
}
