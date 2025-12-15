#include <iostream>

#include "fair_queue.h"
#include "xassert.h"

Request NextRequest(const ClientID client_id) {
  static RequestID next_ = 0;
  return Request{next_++, client_id};
}

void TestFairQueueFunctional() {
  auto fq = RequestFairQueue();
  ASSERT(fq.ClientSize() == 0U);

  // client 0
  fq.Push(std::move(NextRequest(0)));
  ASSERT(fq.ClientSize() == 1U);
  fq.Push(std::move(NextRequest(0)));
  ASSERT(fq.ClientSize() == 1U);

  Request req{0, 0};
  bool ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 0);
  ASSERT(req.request_id == 0);

  ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 0);
  ASSERT(req.request_id == 1);
  ASSERT(fq.ClientSize() == 0U);

  // mutil clients
  fq.Push(std::move(NextRequest(0)));
  ASSERT(fq.ClientSize() == 1U);
  fq.Push(std::move(NextRequest(0)));
  ASSERT(fq.ClientSize() == 1U);

  fq.Push(std::move(NextRequest(1)));
  ASSERT(fq.ClientSize() == 2U);
  fq.Push(std::move(NextRequest(2)));
  ASSERT(fq.ClientSize() == 3U);

  ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 0);
  ASSERT(req.request_id == 2);

  ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 1);
  ASSERT(req.request_id == 4);

  ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 2);
  ASSERT(req.request_id == 5);

  ok = fq.Pop(&req);
  ASSERT(ok);
  ASSERT(req.client_id == 0);
  ASSERT(req.request_id == 3);

  ok = fq.Pop(&req);
  ASSERT(!ok);
  ASSERT(fq.ClientSize() == 0U);
}

int main() {
  TestFairQueueFunctional();
  return 0;
}
