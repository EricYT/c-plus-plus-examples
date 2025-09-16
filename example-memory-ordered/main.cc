#include <atomic>
#include <cassert>
#include <iostream>
#include <string>
#include <thread>

std::atomic<std::string*> ptr;
int data;

// initialize
void init() {
  auto* dummy = new std::string("World");
  ptr = dummy;
  // ptr.store(dummy, std::memory_order_relaxed);

  data = 0;
}

void producer() {
  auto* p = new std::string("Hello");
  data = 42;
  // std::this_thread::sleep_for(std::chrono::seconds(3));
  // std::cout << "producer sleep for 3s" << std::endl;
  ptr.store(p, std::memory_order_release);
  // ptr.store(p, std::memory_order_relaxed);
}

void consumer() {
  std::string* p2;
  while (!(p2 = ptr.load(std::memory_order_acquire)))
    // while (!(p2 = ptr.load(std::memory_order_relaxed)))
    ;
  assert(*p2 == "Hello");
  assert(data == 42);
  // std::cout << "Consumer ptr:" << *p2 << " data:" << data << std::endl;
}

void TestMemoryOrdered() {
  // init();

  std::thread p(producer);
  std::thread c(consumer);

  p.join();
  c.join();
}

int main() {
  TestMemoryOrdered();
  return 0;
}
