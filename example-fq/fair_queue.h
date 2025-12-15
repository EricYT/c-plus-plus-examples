#ifndef __EXAMPLE_FAIR_QUEUE__
#define __EXAMPLE_FAIR_QUEUE__

#include <cstddef>
#include <deque>
#include <unordered_map>

using RequestID = uint64_t;
using ClientID = uint64_t;

struct Request {
  RequestID request_id;
  ClientID client_id;

  explicit Request(RequestID req_id, ClientID client_id) : request_id(req_id), client_id(client_id) {}
};

class ClientReqeustQueue {
 public:
   explicit ClientReqeustQueue(const ClientID client_id);
   ~ClientReqeustQueue();

   void Push(const Request& req);

   bool Pop(Request* req);

   size_t Size() const;

   ClientID GetClientID() const { return client_id_; }

 private:
   ClientID client_id_;
   std::deque<Request> pendings_;
};

class RequestFairQueue {
 public:
   explicit RequestFairQueue();
   ~RequestFairQueue();

   void Push(const Request& req);

   bool Pop(Request* req);

   size_t ClientSize() const { return clients_.size(); }

 private:
   std::unordered_map<ClientID, ClientReqeustQueue*> clients_;
   std::deque<ClientReqeustQueue*> queues_;
};

#endif  // __EXAMPLE_FAIR_QUEUE__
