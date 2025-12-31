#ifndef __EXAMPLE_BLOCK_LIST__
#define __EXAMPLE_BLOCK_LIST__

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>

enum Error : int16_t {
  OK = 0,
  ERROR_CONNECT = -1,
  ERROR_IO_ERROR = -2,
  ERROR_UNKNOWN = -1000,
};

using VDiskId = uint64_t;

enum BlockType : uint8_t {
  BLOCK_TYPE_CONNECT_ERROR = 0,
  BLOCK_TYPE_DISK_ERROR,
  BLOCK_TYPE_NUM,
};

inline std::ostream& operator<<(std::ostream& os, BlockType type) {
  switch (type) {
    case BLOCK_TYPE_CONNECT_ERROR:
      os << "BlockTypeConnectError";
      break;
    case BLOCK_TYPE_DISK_ERROR:
      os << "BlockTypeDiskError";
      break;
    default:
      os << "UnknowBlockType";
  }
  return os;
}

struct BlockPolicy {
  uint64_t record_interval_ms;
  uint32_t block_threhold_cnt;
};

using BlockPolicyTable = std::unordered_map<BlockType, BlockPolicy>;

class BlockList {
 public:
  explicit BlockList(uint32_t cap, const BlockPolicyTable& block_policy)
      : cap_(cap), block_policy_(block_policy) {}

  ~BlockList() {}

  void ReportFailed(VDiskId vdisk_id, int r);

  bool IsBlock(VDiskId vdisk_id) const {
    return block_items_.find(vdisk_id) != block_items_.end();
  }

  void GetBlockList(std::vector<VDiskId>* vdisks);

  void Process();

 private:
  struct BlockItem {
    BlockType type;
    uint64_t block_timestamp;

    explicit BlockItem(BlockType type, uint64_t t)
        : type(type), block_timestamp(t) {}
  };

  using FailedTimestamp = uint64_t;
  using SuspectRecords =
      std::unordered_map<BlockType, std::vector<FailedTimestamp>>;

  static uint64_t Now() {
    return std::chrono::system_clock::now().time_since_epoch().count();
  }

 private:
  const uint32_t cap_;
  const BlockPolicyTable block_policy_;

  std::unordered_map<VDiskId, BlockItem> block_items_;
  std::unordered_map<VDiskId, SuspectRecords> suspect_items_;
};

#endif  // __EXAMPLE_BLOCK_LIST__
