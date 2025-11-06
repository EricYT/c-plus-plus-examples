#pragma once

#include <algorithm>
#include <cctype>
#include <iterator>
#include <string>

inline std::string ltrim(const std::string& s) {
  auto it = std::find_if(s.begin(), s.end(),
                         [](char c) -> bool { return c != ' ' && c != '\t'; });
  return std::string(it, s.end());
}

inline std::string rtrim(const std::string& s) {
  auto it = std::find_if(s.rbegin(), s.rend(),
                         [](char c) -> bool { return c != ' ' && c != '\t'; });
  return std::string(s.begin(), it.base());
}

inline std::string trim(const std::string& s) { return ltrim(rtrim(s)); }

inline std::string toupper(const std::string& s) {
  std::string us;
  std::transform(s.cbegin(), s.cend(), std::back_inserter(us),
                 [](unsigned char c) { return std::toupper(c); });
  return us;
}
