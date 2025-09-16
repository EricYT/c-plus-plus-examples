#include <iostream>
#include <atomic>
#include <cassert>
#include <thread>

template<typename T>
struct Node {
  T value_;
  std::atomic<Node<T>*> next_;

  Node(const T& v) : value_(v), next_(nullptr) {}
};

template<typename T>
class LockFreeQueue {
  public:
    LockFreeQueue() : head_(nullptr), tail_(nullptr) {
      Node<T>* sentinel = new Node<T>(T());
      head_.store(sentinel);
      tail_.store(sentinel);
    }

    ~LockFreeQueue() {
      while (auto* node = head_.load()) {
        head_.store(node->next_.load());
        delete node;
      }
    }

    void enqueue(const T& value) {
      Node<T>* new_node = new Node<T>(value);
      Node<T>* old_tail = tail_.load();
      static Node<T>* null_tail = nullptr;

      while (!old_tail->next_.compare_exchange_weak(
            null_tail,
            new_node,
            std::memory_order_release,
            std::memory_order_relaxed
            )) {
        old_tail = tail_.load();
      }

      tail_.compare_exchange_strong(old_tail, new_node);
    }

    bool dequeue(T& out) {
      auto* old_head = head_.load();
      auto* first_node = old_head->next_.load();

      if (first_node == nullptr) {
        return false;
      }

      out = first_node->value_;

      head_.compare_exchange_strong(old_head, first_node);

      delete old_head;

      return true;
    }

    bool empty() const {
      return head_.load()->next_.load() == nullptr;
    }

  private:
    std::atomic<Node<T>*> head_;
    std::atomic<Node<T>*> tail_;
};

void TestLFQFunctional() {
  LockFreeQueue<int32_t> lfq;

  lfq.enqueue(1);
  lfq.enqueue(2);
  lfq.enqueue(3);
  lfq.enqueue(4);

  assert(!lfq.empty());

  int32_t v = 0;
  assert(lfq.dequeue(v));
  assert(v == 1);

  assert(lfq.dequeue(v));
  assert(v == 2);

  assert(lfq.dequeue(v));
  assert(v == 3);

  assert(lfq.dequeue(v));
  assert(v == 4);

  assert(lfq.empty());
}

void TestLFQConcurrently() {
  LockFreeQueue<int32_t> lfq;

  std::thread producer([&] () -> void {
      for (int i = 0; i < 10; ++i) {
        lfq.enqueue(i);
        std::cout << "Enqueue value:" << i << std::endl;
      }
  });

  std::thread consumer([&] () -> void {
      for (int i = 0; i < 10; ++i) {
        int value = 0;
        while (!lfq.dequeue(value)) {
          // wait for new one
        }
        std::cout << "Dequeue value:" << value << std::endl;
      }
  });

  producer.join();
  consumer.join();
}

int main() {
  TestLFQFunctional();
  TestLFQConcurrently();
  return 0;
}
