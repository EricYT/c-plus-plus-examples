#include <atomic>
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::atomic<std::string*> ptr;
int data;

struct Foo {
  void operator()(int32_t i) { std::cout << "index " << i << std::endl; }
};
// call it like foo(32);

void producer() {
  auto* p = new std::string("Hello");
  data = 42;
  ptr.store(p, std::memory_order_release);
}

void consumer() {
  std::string* p2;
  while (!(p2 = ptr.load(std::memory_order_acquire)));
  assert(*p2 == "Hello");
  assert(data == 42);
}

void TestMemoryOrdered() {
  std::thread p(producer);
  std::thread c(consumer);

  p.join();
  c.join();
}

// ------------ acl_rel
std::atomic<int> completed_tasks{0};
const int TOTAL_TASKS = 5;

void on_all_tasks_completed() {
  std::cout << "All tasks completed." << std::endl;
}

void perform_task(int task_id) {
  std::this_thread::sleep_for(std::chrono::milliseconds(100 * task_id));
  std::cout << "Task " << task_id << " already completed." << std::endl;

  int prev = completed_tasks.fetch_add(1, std::memory_order_acq_rel);
  if (prev + 1 == TOTAL_TASKS) {
    on_all_tasks_completed();
  }
}

void TestMmeoryOrderedAcqRel() {
  std::vector<std::thread> threads;

  for (int i = 0; i < TOTAL_TASKS; ++i) {
    threads.emplace_back(perform_task, i);
  }

  for (auto& t : threads) {
    t.join();
  }
}

int main() {
  TestMemoryOrdered();
  TestMmeoryOrderedAcqRel();
  return 0;
}
