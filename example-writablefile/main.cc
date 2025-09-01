#include <functional>
#include <iostream>
#include <string>

#include "writable_file.h"

void TestWritableFile() {
  constexpr const char* fname = "./hello.txt";
  WritableFile* file;
  Status s = NewWritableFile(fname, &file);
  assert(s.ok());
  s = file->Append("hello, world");
  assert(s.ok());
  s = file->Close();
  assert(s.ok());
}

int main() {
  TestWritableFile();
  return 0;
}
