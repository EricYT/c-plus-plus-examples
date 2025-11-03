#include <iostream>

#include "options.h"

void TestIncOptions() {
  Options opts;
  std::cout << "filename:" << opts.file_name << std::endl;
  std::cout << "file num:" << opts.file_number << std::endl;

  opts.set_file_name("/etc/options/options.conf");
  std::cout << "filename:" << opts.file_name << std::endl;
  opts.set_file_number(2048);
  std::cout << "file num:" << opts.file_number << std::endl;
}

int main() {
  TestIncOptions();
  return 0;
}
