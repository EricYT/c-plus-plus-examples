#pragma  once

#include <cstddef>
#include <string>
#include <cassert>

class Slice {
  public:
    Slice() : data_(nullptr), size_(0) {}

    Slice(const char* data, size_t size) : data_(data), size_(size) {}

    Slice(const std::string& s) : data_(s.c_str()), size_(s.size()) {}

    Slice(const char* s) : data_(s), size_(strlen(s)) {}

    Slice(const Slice&) = default;
    Slice& operator=(const Slice&) = default;

    const char* data() const { return data_; }

    size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    const char* begin() const { return data(); }
    const char* rbegin() const { return data() + size_; }

    char operator[](size_t n) const {
      assert(n < size_);
      return data_[n];
    }

    void clear() {
      data_ = nullptr;
      size_ = 0 ;
    }

    void remove_prefix(size_t n) {
      assert(n <= size_);
      data_ += n;
      size_ -= n;
    }

    std::string to_string() const { return std::string(data_, size_); }

    // three way comparsion. Return values
    // < 0 iff "this" < "b"
    // == 0 iff "this" == "b"
    // > 0 iff "this" > "b"
    int compare(const Slice& b) const;

    // Returns true iff "x" is a prefix of "*this"
    bool starts_with(const Slice& x) const {
      return (size_ >= x.size_) && (std::memcmp(data_, x.data_, x.size_) == 0);
    }

  private:
    const char* data_;
    size_t size_;
};

inline bool operator==(const Slice& a, const Slice& b) {
  return (a.size() == b.size()) &&
    (std::memcmp(a.data(), b.data(), a.size()) == 0);
}

inline bool operator!=(const Slice& a, const Slice& b) { return !(a == b); }

inline int Slice::compare(const Slice &b) const {
  const size_t min_size = size_ > b.size_ ? b.size_ : size_;
  int r = std::memcmp(data_, b.data_, min_size);
  if (r == 0) {
    if (size_ < b.size_) {
      r = -1;
    } else if (size_ > b.size_) {
      r = +1;
    }
  }
  return r;
}
