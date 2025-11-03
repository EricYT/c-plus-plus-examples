#pragma once

#include "slice.h"
#include "status.h"

namespace WAL {

using AppendCallback = void(*)(Status status, int size);

class RingWAL {
 public:
   virtual ~RingWAL() {}

   // iterator wal from pos 0

   virtual Status Init(size_t head, size_t tail) = 0;

   virtual Status Append(const Slice& data, AppendCallback cb) = 0;

   virtual Status AdvanceHead(uint64_t size) = 0;

   virtual void SetTail(uint64_t pos) = 0;

   virtual void SetHead(uint64_t pos) = 0;

   virtual size_t Size() = 0;
};

class FileRingWAL : public RingWAL {
 public:
   FileRingWAL();

   virtual ~FileRingWAL() override {}

   virtual Status Init(size_t head, size_t tail) override;

   Status Append(const Slice& data, AppendCallback cb) override;

   Status AdvanceHead(uint64_t size) override;

   void SetTail(uint64_t pos) override;

   void SetHead(uint64_t pos) override;

   size_t Size() override;

 private:
};

// for testing
class MemRingWAL : public RingWAL {
 public:
   MemRingWAL(uint64_t cap);

   virtual ~MemRingWAL() override {
     if (buf_) {
       delete []buf_;
     }
   }

   Status Append(const Slice& data, AppendCallback cb) override;

   Status AdvanceHead(uint64_t size) override;

   void SetTail(uint64_t pos) override;

   void SetHead(uint64_t pos) override;

   size_t Size() override;
 private:
   uint64_t cap_;

   char* const buf_;
};

};  // namespace WAL
