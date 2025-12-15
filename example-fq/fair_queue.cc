#include "fair_queue.h"

#include "xassert.h"

ClientReqeustQueue::ClientReqeustQueue(const ClientID client_id)
    : client_id_(client_id) {}

ClientReqeustQueue::~ClientReqeustQueue() {}

void ClientReqeustQueue::Push(const Request& req) {
  ASSERT(req.client_id == client_id_);
  pendings_.push_back(req);
}

bool ClientReqeustQueue::Pop(Request* req) {
  ASSERT(!pendings_.empty());
  const auto& front = pendings_.front();
  pendings_.pop_front();
  *req = front;
  return pendings_.empty();
}

size_t ClientReqeustQueue::Size() const { return pendings_.size(); }

RequestFairQueue::RequestFairQueue() {}

RequestFairQueue::~RequestFairQueue() {
  for (auto* q : queues_) {
    delete q;
  }
  clients_.clear();
  queues_.clear();
}

void RequestFairQueue::Push(const Request& req) {
  const auto& iter = clients_.find(req.client_id);
  if (iter == clients_.end()) {
    auto* client_queue = new ClientReqeustQueue(req.client_id);
    client_queue->Push(req);
    clients_.emplace(req.client_id, client_queue);
    queues_.emplace_back(client_queue);
    return;
  }
  iter->second->Push(req);
}

bool RequestFairQueue::Pop(Request* req) {
  if (clients_.empty()) {
    return false;
  }
  auto& front = queues_.front();
  queues_.pop_front();
  bool empty = front->Pop(req);
  if (empty) {
    clients_.erase(front->GetClientID());
    return true;
  }
  queues_.emplace_back(front);
  return true;
}
