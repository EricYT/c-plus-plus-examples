#pragma once

#include <functional>
#include <utility>

class Defer {
 public:
  explicit Defer(std::function<void()> func) : func_(std::move(func)) {}
  ~Defer() {
    if (func_) func_();
  }

  Defer(const Defer&) = delete;
  Defer& operator=(const Defer&) = delete;

 private:
  std::function<void()> func_;
};

#define CONCAT(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x##y
#define DEFER(...) Defer CONCAT(__defer_, __LINE__)([]() { __VA_ARGS__; })

template <typename Func>
class deferred_action {
 public:
  explicit deferred_action(Func&& func) : func_(std::move(func)) {}
  ~deferred_action() { func_(); }

 private:
  Func func_;
};

template <typename Func>
inline deferred_action<Func> defer(Func&& func) {
  return deferred_action<Func>(std::forward<Func>(func));
}
