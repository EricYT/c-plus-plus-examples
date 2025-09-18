#pragma once

#include "port_stdcxx.h"
#include "thread_annotations.h"

namespace example {

class SCOPED_LOCKABLE MutexLock {
 public:
  explicit MutexLock(port::Mutex* mu) EXCLUSIVE_LOCK_FUNCTION(mu) : mu_(mu) {
    mu_->Lock();
  }
  ~MutexLock() UNLOCK_FUNCTION() {
    if (mu_ != nullptr) {
      mu_->Unlock();
    }
  }

  MutexLock(const MutexLock&) = delete;
  MutexLock& operator=(const MutexLock&) = delete;

 private:
  port::Mutex* const mu_;
};

}  // namespace example
