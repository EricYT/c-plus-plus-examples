#pragma once

#include <cstdint>

namespace example {

enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };

typedef uint64_t SequenceNumber;

}  // namespace example
