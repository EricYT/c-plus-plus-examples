#include <stdint.h>
#include <cstring>
#include <utility>
#include <cassert>

#include "status.h"

Status::Status(const Status& rhs) {
  state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

Status& Status::operator=(const Status& rhs) {
  if (state_ != rhs.state_) {
    delete[] state_;
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }
  return *this;
}

Status& Status::operator=(Status&& rhs) noexcept {
  std::swap(state_, rhs.state_);
  return *this;
}

std::string Status::ToString() const {
  if (state_ == nullptr) {
    return "OK";
  } else {
    char tmp[30];
    const char* type;
    switch (code()) {
      case kOk:
        type = "OK";
        break;
      case kNotFound:
        type = "NotFound: ";
        break;
      case kCorruption:
        type = "Corruption: ";
        break;
      case kNotSupported:
        type = "NotSupported: ";
        break;
      case kInvalidArgument:
        type = "InvalidArgument: ";
        break;
      case kIOError:
        type = "IOError: ";
        break;
      default:
        std::snprintf(tmp, sizeof(tmp),
            "Unknow code(%d)", static_cast<int>(code()));
        type = tmp;
        break;
    }
    std::string result(type);
    uint32_t msg_len;
    std::memcpy(&msg_len, state_, sizeof(msg_len));
    result.append(state_+5, msg_len);
    return result;
  }
}

const char* Status::CopyState(const char* state) {
  uint32_t size;
  std::memcpy(&size, state, sizeof(size));
  char* result = new char[size + 5];
  std::memcpy(result, state, size + 5);
  return result;
}

Status::Status(Code code, const Slice& msg1, const Slice& msg2) {
  assert(code != kOk);
  const uint32_t len1 = static_cast<uint32_t>(msg1.size());
  const uint32_t len2 = static_cast<uint32_t>(msg2.size());
  const uint32_t size = len1 + (len2 ? len2 + 2 : 0);
  char* result = new char[size + 5];
  std::memcpy(result, &size, sizeof(size));
  result[4] = static_cast<char>(code);
  std::memcpy(result+5, msg1.data(), len1);
  if (len2) {
    result[5 + len1] = ':';
    result[6 + len1] = ' ';
    std::memcpy(result+5+len1+2, msg2.data(), msg2.size());
  }
  state_ = result;
}
