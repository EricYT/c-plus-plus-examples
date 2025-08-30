#pragma once

#include <string>

#include "slice.h"

class Status {
  public:
    Status() noexcept : state_(nullptr) {}
    ~Status() { delete[] state_; }

    Status(const Status& rhs);
    Status& operator=(const Status& rhs);

    Status(Status&& rhs) : state_(rhs.state_) { rhs.state_ = nullptr; }
    Status& operator=(Status&& rhs) noexcept;

    static Status OK() { return Status(); }

    static Status NotFound(const Slice& msg1, const Slice& msg2 = Slice()) {
      return Status(kNotFound, msg1, msg2);
    }

    static Status Corruption(const Slice& msg1, const Slice& msg2 = Slice()) {
      return Status(kCorruption, msg1, msg2);
    }

    static Status NotSupported(const Slice& msg1, const Slice& msg2 = Slice()) {
      return Status(kNotSupported, msg1, msg2);
    }

    static Status InvalidArgument(const Slice& msg1, const Slice& msg2 = Slice()) {
      return Status(kInvalidArgument, msg1, msg2);
    }

    static Status IOError(const Slice& msg1, const Slice& msg2 = Slice()) {
      return Status(kIOError, msg1, msg2);
    }

    bool ok() const { return state_ == nullptr; }

    bool IsNotFound() const { return code() == kNotFound; }

    bool IsCorruption() const { return code() == kCorruption; }

    bool IsNotSupported() const { return code() == kNotSupported; }

    bool IsInvalidArgument() const { return code() == kInvalidArgument; }

    bool IsIOError() const { return code() == kIOError; }

    std::string ToString() const;

  private:
    enum Code {
      kOk = 0,
      kNotFound = 1,
      kCorruption = 2,
      kNotSupported = 3,
      kInvalidArgument = 4,
      kIOError = 5
    };

    Code code() const {
      return (state_ == nullptr ? kOk : static_cast<Code>(state_[4]));
    }

    static const char* CopyState(const char* s);

    Status(Code code, const Slice& msg1, const Slice& msg2);

    // OK status has a null state_. Otherwise is a new[] array
    // of the following form:
    // state_[0..3] == length of msg
    // state_[4]    == code
    // state_[5..]  == msg
    const char* state_;
};
