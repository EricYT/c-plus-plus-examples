#ifndef __EXAMPLE_MIN_HEAP__
#define __EXAMPLE_MIN_HEAP__

#include <algorithm>
#include <queue>
#include <vector>

template <typename T, typename Container = std::vector<T>,
          typename Compare = std::greater<T>>
struct MinHeap : std::priority_queue<T, Container, Compare> {
  const Container& GetData() const { return this->c; }
};

template <typename T>
void top_k_min(const MinHeap<T>& heap, size_t k, std::vector<T>* result) {
  ASSERT(result != nullptr);
  if (heap.empty() || k == 0) return;

  size_t n = heap.size();
  k = std::min(n, k);

  const auto& heap_data = heap.GetData();

  using Candidate = std::pair<T, size_t>;
  std::priority_queue<Candidate, std::vector<Candidate>,
                      std::greater<Candidate>>
      min_candidate;

  min_candidate.emplace(heap_data[0], 0);

  for (size_t i = 0; i < k; ++i) {
    const auto [val, idx] = min_candidate.top();
    min_candidate.pop();
    result->push_back(val);

    size_t left = 2 * idx + 1;
    if (left < n) {
      min_candidate.emplace(heap_data[left], left);
    }

    size_t right = 2 * idx + 2;
    if (right < n) {
      min_candidate.emplace(heap_data[right], right);
    }
  }
}

#endif  // __EXAMPLE_MIN_HEAP__
