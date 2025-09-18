#pragma once

#include <cstddef>
#include <string>

#include "dbformat.h"
#include "slice.h"

namespace example {

class WriteBatchInternal;

class WriteBatch {
 public:
  WriteBatch();
  ~WriteBatch();

  void Put(const Slice& key, const Slice& value);

  void Delete(const Slice& key);

  void Clear();

  size_t ApproximateSize() const;

  void Append(const WriteBatch& source);

 private:
  friend class WriteBatchInternal;

  std::string rep_;
};

class WriteBatchInternal {
 public:
  // Return the number of entries in the batch
  static int Count(const WriteBatch* batch);

  // Set the count for the number of entries in the batch
  static void SetCount(WriteBatch* batch, int n);

  // Return the sequence number for the first entry of this batch
  static SequenceNumber Sequence(const WriteBatch* batch);

  static void SetSequence(WriteBatch* batch, SequenceNumber seq);

  static Slice Contents(const WriteBatch* batch) { return Slice(batch->rep_); }

  static size_t ByteSize(const WriteBatch* batch) { return batch->rep_.size(); }

  static void SetContents(WriteBatch* batch, const Slice& contents);

  // static Status InsertInto(const WriteBatch* batch, MemTable* memtable);

  static void Append(WriteBatch* dst, const WriteBatch* src);
};

}  // namespace example
