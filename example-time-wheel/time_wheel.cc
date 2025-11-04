
#include "time_wheel.h"

bool TimeWheel::AddTimer(Timer* t) {
  if (t == nullptr || !t->active) {
    return false;
  }

  jiffies now = Now();
  jiffies delta = 0ULL;
  if (t->expires <= now) {
    delta = 0ULL;
  } else {
    delta = t->expires - now;
  }
  if (delta >= (1ULL << (TVN_NUM * TVN_BITS))) {
    return false;
  }

  int level = GetLevel(delta);
  int slot_idx = 0;
  if (level == 0) {
    slot_idx = delta & (TVN_SIZE - 1);
  } else {
    int shift = TVN_BITS * level;
    int adjusted_delta = delta - (1ULL << shift);
    slot_idx = (adjusted_delta >> shift) & (TVN_SIZE - 1);
  }

  std::cout << "expires:" << t->expires << " level:" << level
            << " slot:" << slot_idx << std::endl;
  tv_[level][slot_idx].push_back(t);

  return true;
}

void TimeWheel::RemoveTimer(Timer* t) {
  if (!t) t->active = false;
}

void TimeWheel::Tick() {
  jiffies now = ++now_;

  int level = 0;

  auto& curr_slot = tv_[level][tv_idx_[level]];

  std::list<Timer*> expired;
  for (auto it = curr_slot.begin(); it != curr_slot.end();) {
    auto* t = *it;
    if (!t->active) {
      it = curr_slot.erase(it);
      continue;
    }
    if (t->expires <= now) {
      it = curr_slot.erase(it);
      expired.push_back(t);
      t->active = false;
    } else {
      ++it;
    }
  }
  for (auto* t : expired) {
    if (t->func) t->func(t->data);
  }

  tv_idx_[level] = (tv_idx_[level] + 1) % TVN_SIZE;
  if (tv_idx_[level] == 0) {
    Cascade(1);
  }
}

int TimeWheel::GetLevel(jiffies delta) const {
  if (delta < (1 << TVN_BITS)) return 0;        // [0, 255]
  if (delta < (1 << (TVN_BITS * 2))) return 1;  // [256, 131071]
  if (delta < (1 << (TVN_BITS * 3))) return 2;  // [131072, 33554431]
  return 3;                                     // [33554432, 8589934591]
}

void TimeWheel::Cascade(int level) {
  std::cout << "Cascade level:" << level << " slot:" << tv_idx_[level]
            << std::endl;
  if (level >= TVN_NUM) return;

  auto& curr_slot = tv_[level][tv_idx_[level]];
  std::list<Timer*> timers;
  timers.swap(curr_slot);

  for (auto* t : timers) {
    if (t->active) {
      AddTimer(t);
    }
  }

  tv_idx_[level] = (tv_idx_[level] + 1) % TVN_SIZE;
  if (tv_idx_[level] == 0 && level + 1 < TVN_NUM) {
    Cascade(level + 1);
  }
}
