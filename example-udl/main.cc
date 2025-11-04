#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>

// User-Defined Literals

constexpr unsigned long long operator"" _K(unsigned long long n) {
  return n << 10;
}
constexpr unsigned long long operator"" _M(unsigned long long n) {
  return n << 20;
}
constexpr unsigned long long operator"" _G(unsigned long long n) {
  return n << 30;
}
constexpr unsigned long long operator"" _T(unsigned long long n) {
  return n << 40;
}

void TestNumUDLFunctional() {
  assert(1_K == 1ULL * 1024);
  assert(2_K == 2ULL * 1024);
  assert(1_M == 1ULL * 1024 * 1024);
  assert(3_M == 3ULL * 1024 * 1024);
  assert(1_G == 1ULL * 1024 * 1024 * 1024);
  assert(4_G == 4ULL * 1024 * 1024 * 1024);
  assert(1_T == 1ULL * 1024 * 1024 * 1024 * 1024);
  assert(5_T == 5ULL * 1024 * 1024 * 1024 * 1024);
}

constexpr int operator"" _hex(const char* str, std::size_t _) {
  return std::strtoul(str, nullptr, 16);
}

void TestStrUDLFunctional() { assert("1a"_hex == 26); }

constexpr long double operator"" _kg(long double n) {
  return n * 1000;  // kg to g
}

void TestKGUDLFunctional() { assert(0.5_kg == 500); }

int main() {
  TestNumUDLFunctional();
  TestStrUDLFunctional();
  TestKGUDLFunctional();
  return 0;
}
