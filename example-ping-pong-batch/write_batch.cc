#include "write_batch.h"

#include "coding.h"
#include "dbformat.h"

// WriteBatch::rep_ :=
//     sequence: fixed64
//     count: fixed32
//     data: record[count]
// record :=
//     kTypeValue varstring varstring
//     kTypeDelete varstring
// varstring :=
//     len: varint32
//     data: uint8[len]

namespace example {

// WriteBatch header has an 8-byte sequence number followed by a 4-byte count.
static const size_t kHeader = 12;

WriteBatch::WriteBatch() { Clear(); }

WriteBatch::~WriteBatch() = default;

void WriteBatch::Clear() {
  rep_.clear();
  rep_.resize(kHeader);
}

size_t WriteBatch::ApproximateSize() const { return rep_.size(); }

void WriteBatch::Put(const Slice& key, const Slice& value) {
  WriteBatchInternal::SetCount(this, WriteBatchInternal::Count(this) + 1);
  rep_.push_back(static_cast<char>(kTypeValue));
  PutLengthPrefixedSlice(&rep_, key);
  PutLengthPrefixedSlice(&rep_, value);
}

void WriteBatch::Delete(const Slice& key) {
  WriteBatchInternal::SetCount(this, WriteBatchInternal::Count(this) + 1);
  rep_.push_back(static_cast<char>(kTypeDeletion));
  PutLengthPrefixedSlice(&rep_, key);
}

int WriteBatchInternal::Count(const WriteBatch* batch) {
  return DecodeFixed32(batch->rep_.c_str() + 8);
}

void WriteBatchInternal::SetCount(WriteBatch* batch, int n) {
  EncodeFixed32(&batch->rep_[8], n);
}

SequenceNumber WriteBatchInternal::Sequence(const WriteBatch* batch) {
  return DecodeFixed64(batch->rep_.c_str());
}

void WriteBatchInternal::SetSequence(WriteBatch* batch, SequenceNumber seq) {
  EncodeFixed64(&batch->rep_[0], seq);
}

void WriteBatchInternal::SetContents(WriteBatch* batch, const Slice& contents) {
  batch->rep_.assign(contents.data(), contents.size());
}

void WriteBatchInternal::Append(WriteBatch* dst, const WriteBatch* src) {
  SetCount(dst, Count(dst) + Count(src));
  dst->rep_.append(src->rep_.c_str() + kHeader, src->rep_.size() - kHeader);
}

}  // namespace example
