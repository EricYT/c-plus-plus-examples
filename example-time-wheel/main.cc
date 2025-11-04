#include <iostream>

#include "time_wheel.h"

void TestTimeWheel() {
  TimeWheel tw;

  // 定时器1：5ms后触发
  Timer t1(
      tw.Now() + 5,
      [&](void* data) {
        (void)data;
        std::cout << "Timer 1 expired at jiffies=" << tw.Now() << std::endl;
      },
      nullptr);

  // 定时器2：300ms后触发
  Timer t2(
      tw.Now() + 300,
      [&](void* data) {
        (void)data;
        std::cout << "Timer 2 expired at jiffies=" << tw.Now() << std::endl;
      },
      nullptr);

  // 定时器3：552ms后触发
  Timer t3(
      tw.Now() + 552,
      [&](void* data) {
        (void)data;
        std::cout << "Timer 3 expired at jiffies=" << tw.Now() << std::endl;
      },
      nullptr);

  // 定时器4：66048ms后触发
  Timer t4(
      tw.Now() + 66048,
      [&](void* data) {
        (void)data;
        std::cout << "Timer 4 expired at jiffies=" << tw.Now() << std::endl;
      },
      nullptr);

  tw.AddTimer(&t1);
  tw.AddTimer(&t2);
  tw.AddTimer(&t3);
  tw.AddTimer(&t4);

  // 模拟1000个节拍
  for (int i = 0; i < 70000; ++i) {
    tw.Tick();
    std::cout << "Current jiffies: " << tw.Now() << std::endl;
  }
}

int main() {
  TestTimeWheel();
  return 0;
}
