#include <iostream>

#include "block_list.h"
#include "xassert.h"

void TestBlockListWithoutBlockPolicy() {
  BlockList bl(3, {});
  std::vector<VDiskId> vdisks;
  bl.GetBlockList(&vdisks);
  ASSERT(vdisks.empty());

  // no block policy
  bl.ReportFailed(0, ERROR_IO_ERROR);
  bl.ReportFailed(0, ERROR_IO_ERROR);
  bl.ReportFailed(1, ERROR_CONNECT);
}

void TestBlockListFunctional() {
  BlockPolicyTable tb;
  tb[BLOCK_TYPE_CONNECT_ERROR] = {3000, 2};
  tb[BLOCK_TYPE_DISK_ERROR] = {0, 1};
  BlockList bl(3, tb);

  // empty
  std::vector<VDiskId> vdisks;
  bl.GetBlockList(&vdisks);
  ASSERT(vdisks.empty());

  bl.ReportFailed(3, ERROR_IO_ERROR);
  bl.GetBlockList(&vdisks);
  ASSERT(vdisks.size() == 1U);
  ASSERT(vdisks[0] == 3);

  bl.ReportFailed(3, ERROR_IO_ERROR);
  bl.GetBlockList(&vdisks);
  ASSERT(vdisks.size() == 1U);
  ASSERT(vdisks[0] == 3);
}

int main() {
  TestBlockListWithoutBlockPolicy();
  TestBlockListFunctional();
}
