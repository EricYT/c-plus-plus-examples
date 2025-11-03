#include <cassert>
#include <cerrno>
#include <chrono>
#include <deque>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "mutexlock.h"
#include "port_stdcxx.h"
#include "slice.h"
#include "status.h"
#include "thread_annotations.h"
#include "write_batch.h"

namespace example {

class DBImpl {
 public:
  DBImpl() = default;
  ~DBImpl() = default;

  DBImpl(const DBImpl&) = delete;
  DBImpl& operator=(const DBImpl&) = delete;

  Status Write(WriteBatch* updates);

 private:
  struct Writer {
    explicit Writer(port::Mutex* mu) : done_(false), cv_(mu) {}

    WriteBatch* batch_;
    bool done_;
    Status status_;
    port::CondVar cv_;
  };

  WriteBatch* BuildBatchGroup(Writer** last_writer);

  port::Mutex mu_;
  std::deque<Writer*> writers_ GUARDED_BY(mu_);
};

Status DBImpl::Write(WriteBatch* updates) {
  assert(updates != nullptr);

  Writer w(&mu_);
  w.batch_ = updates;
  w.done_ = false;

  example::MutexLock l(&mu_);
  writers_.push_back(&w);
  while (!w.done_ && writers_.front() != &w) {
    w.cv_.Wait();
  }

  if (w.done_) {
    return w.status_;
  }

  std::cout << "DBImpl::Write fired by" << std::endl;

  // fire
  Status status;
  auto* last_writer = &w;
  auto* batch_group = BuildBatchGroup(&last_writer);
  {
    mu_.Unlock();

    std::cout << "DBImpl::Write something like write wal logs" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "DBImpl::Write batch cnt:"
              << WriteBatchInternal::Count(batch_group) << std::endl;

    status = Status::OK();

    mu_.Lock();
  }

  while (true) {
    Writer* ready = writers_.front();
    writers_.pop_front();
    if (ready != &w) {
      ready->done_ = true;
      ready->status_ = status;
      ready->cv_.Signal();
    }
    if (ready == last_writer) break;
  }

  if (!writers_.empty()) {
    writers_.front()->cv_.Signal();
  }

  return status;
}

WriteBatch* DBImpl::BuildBatchGroup(Writer** last_writer) {
  mu_.AssertHeld();
  assert(!writers_.empty());
  Writer* first = writers_.front();
  WriteBatch* result = first->batch_;
  assert(result != nullptr);

  *last_writer = first;
  std::deque<Writer*>::iterator iter = writers_.begin();
  ++iter;
  for (; iter != writers_.end(); ++iter) {
    static const int kMaxBatchSize = 2;
    if (WriteBatchInternal::Count(result) >= kMaxBatchSize) break;
    Writer* w = *iter;
    WriteBatchInternal::Append(result, w->batch_);
    *last_writer = w;
  }

  return result;
}

}  // namespace example

void TestPingPongBatchFunctional() {
  example::DBImpl dbimpl;

  // write
  example::WriteBatch w;
  w.Put("k0", "hello");
  w.Put("k1", "world");
  w.Delete("k0");
  Status s = dbimpl.Write(&w);
  assert(s.ok());
}

void TestPingPongBatch() {
  example::DBImpl dbimpl;

  example::WriteBatch w0;
  w0.Put("k0", "v0");

  example::WriteBatch w1;
  w1.Put("k1", "v1");

  example::WriteBatch w2;
  w2.Put("k2", "v2");

  example::WriteBatch w3;
  w3.Put("k3", "v3");

  std::thread t0([&]() { dbimpl.Write(&w0); });
  std::thread t1([&]() { dbimpl.Write(&w1); });
  std::thread t2([&]() { dbimpl.Write(&w2); });
  std::thread t3([&]() { dbimpl.Write(&w3); });

  t0.join();
  t1.join();
  t2.join();
  t3.join();
}

int main() {
  // TestPingPongBatchFunctional();
  TestPingPongBatch();
  return 0;
}
