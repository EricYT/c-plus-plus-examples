#pragma once

#include <iostream>
#include <list>
#include <cstdint>
#include <functional>

using jiffies = uint64_t;
using TimerFunc = std::function<void(void*)>;

struct Timer {
  jiffies expires;
  bool active;
  TimerFunc func;
  void* data;

  explicit Timer(jiffies expires, TimerFunc&& func, void* d) :
    expires(expires), active(true), func(std::move(func)), data(d) {}
};

class TimeWheel {
 public:
   TimeWheel() : now_(0) {}

   ~TimeWheel() = default;

   bool AddTimer(Timer* t);

   void RemoveTimer(Timer* t);

   void Tick();

   jiffies Now() const { return now_; }
 private:

   int GetLevel(jiffies delta) const;

   void Cascade(int level);

 private:
   static const int TVN_BITS = 8;
   static const int TVN_SIZE = 1ULL << TVN_BITS;
   static const int TVN_NUM = 4;

   std::list<Timer*> tv_[TVN_NUM][TVN_SIZE];
   int tv_idx_[TVN_NUM] = {0};

   jiffies now_;
};
