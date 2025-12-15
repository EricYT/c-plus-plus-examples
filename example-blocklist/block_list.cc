#include <iostream>

#include "block_list.h"

static BlockType ConvertErrorToBlockType(int r) {
  switch (r) {
  case ERROR_CONNECT:
    return BLOCK_TYPE_CONNECT_ERROR;
  case ERROR_IO_ERROR:
    return BLOCK_TYPE_DISK_ERROR;
  default:
    return BLOCK_TYPE_NUM;
  }
}

void BlockList::ReportFailed(VDiskId vdisk_id, int r) {
  std::cout << "ReportFailed vdisk_id:" << vdisk_id << " error:" << r << std::endl;
  BlockType type = ConvertErrorToBlockType(r);
  if (type == BLOCK_TYPE_NUM) {
    std::cout << "ReportFailed vdisk_id:" << vdisk_id << " unknow error:" << r << std::endl;
    return;
  }
  if (IsBlock(vdisk_id)) {
    std::cout << "ReportFailed vdisk already blocked. vdisk_id:" << vdisk_id << std::endl;
    return;
  }
  const auto& bp_iter = block_policy_.find(type);
  if (bp_iter == block_policy_.end()) {
    std::cout << "ReportFailed vdisk_id:" << vdisk_id << " unable to find policy for " << type << std::endl;
    return;
  }

  // update failed record
  suspect_items_[vdisk_id][type].emplace_back(Now());

  auto& records = suspect_items_[vdisk_id];
  if (bp_iter->second.record_interval_ms > 0) {
    auto& faileds = records[type];
    auto curr = Now();
    auto ttl = curr - bp_iter->second.record_interval_ms;
    for (auto iter = faileds.begin(); iter != faileds.end();) {
      if (*iter < ttl) {
        iter = faileds.erase(iter);
      } else {
        ++iter;
      }
    }
    if (faileds.size() < bp_iter->second.block_threhold_cnt) {
      return;
    }
  }
  BlockItem bi(type, Now());
  block_items_.emplace(vdisk_id, bi);
  records.erase(type);
  if (records.empty()) {
    suspect_items_.erase(vdisk_id);
  }
}

void BlockList::GetBlockList(std::vector<VDiskId>* vdisks) {
  vdisks->clear();
  for (const auto& block : block_items_) {
    vdisks->emplace_back(block.first);
  }
}

void BlockList::Process() {
}
