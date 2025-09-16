#include <dirent.h>
#include <sys/types.h>

#include <cassert>
#include <cerrno>
#include <iostream>
#include <vector>

namespace FOO {
struct DIR {};

errno_t GetChildren(const std::string& directory_path,
                    std::vector<std::string>* result) {
  result->clear();

  ::DIR* dir = ::opendir(directory_path.c_str());
  if (dir == nullptr) {
    return errno;
  }
  struct ::dirent* entry = nullptr;
  while ((entry = ::readdir(dir)) != nullptr) {
    result->push_back(entry->d_name);
  }
  ::closedir(dir);
  return 0;
}

void TestGetChildren() {
  std::vector<std::string> result;
  auto res = GetChildren(".", &result);
  assert(res == 0);
  for (const auto& name : result) {
    std::cout << name << std::endl;
  }
}

};  // namespace FOO

int main() {
  FOO::TestGetChildren();
  return 0;
}
