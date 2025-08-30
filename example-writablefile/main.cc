#include <functional>
#include <iostream>
#include <string>

#include "writable_file.h"

void TestWritableFile() {
  WritableFileImpl wf("xxx", 0);
}

int main() {
  TestWritableFile();
  return 0;
}
