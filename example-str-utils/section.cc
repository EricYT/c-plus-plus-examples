#include "section.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "string.h"

template <>
int32_t Section::Get<int32_t>(const std::string& key,
                              const int32_t& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }
  try {
    long val = std::stol(it->second);
    if (val < std::numeric_limits<int32_t>::min() ||
        val > std::numeric_limits<int32_t>::max()) {
      return def;
    }
    return static_cast<int32_t>(val);
  } catch (const std::invalid_argument&) {
    return def;
  } catch (const std::out_of_range&) {
    return def;
  }
}

template <>
int64_t Section::Get<int64_t>(const std::string& key,
                              const int64_t& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }
  try {
    long val = std::stoll(it->second);
    if (val < std::numeric_limits<int64_t>::min() ||
        val > std::numeric_limits<int64_t>::max()) {
      return def;
    }
    return static_cast<int64_t>(val);
  } catch (const std::invalid_argument&) {
    return def;
  } catch (const std::out_of_range&) {
    return def;
  }
}

template <>
uint32_t Section::Get<uint32_t>(const std::string& key,
                                const uint32_t& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }
  try {
    long val = std::stoul(it->second);
    if (val < std::numeric_limits<uint32_t>::min() ||
        val > std::numeric_limits<uint32_t>::max()) {
      return def;
    }
    return static_cast<uint32_t>(val);
  } catch (const std::invalid_argument&) {
    return def;
  } catch (const std::out_of_range&) {
    return def;
  }
}

template <>
uint64_t Section::Get<uint64_t>(const std::string& key,
                                const uint64_t& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }
  try {
    long val = std::stoul(it->second);
    if (val < std::numeric_limits<uint64_t>::min() ||
        val > std::numeric_limits<uint64_t>::max()) {
      return def;
    }
    return static_cast<uint64_t>(val);
  } catch (const std::invalid_argument&) {
    return def;
  } catch (const std::out_of_range&) {
    return def;
  }
}

template <>
bool Section::Get<bool>(const std::string& key, const bool& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }

  static const std::string TRUE = "TRUE";
  static const std::string FALSE = "FALSE";

  const std::string val = toupper(it->second);
  if (val == TRUE) return true;
  if (val == FALSE) return false;
  return def;
}

template <>
std::string Section::Get<std::string>(const std::string& key,
                                      const std::string& def) const {
  auto it = data_.find(key);
  if (it == data_.end()) {
    return def;
  }
  return it->second;
}

bool ParseSectionFile(const std::string& filename, Sections* sections) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  std::string line;
  Section curr;
  while (std::getline(file, line)) {
    line = trim(line);
    if (line.empty()) continue;

    if (line[0] == '#' || line[0] == '"') continue;  // comments

    // new section
    if (line[0] == '[' && line[line.size() - 1] == ']') {
      if (!curr.Empty()) {
        sections->emplace(curr.Name(), curr);
        curr.Clear();
      }
      curr.SetName(line.substr(1, line.size() - 2));
      continue;
    }

    size_t equal_pos = line.find('=');
    // wrong line without '='
    if (equal_pos == std::string::npos) continue;

    std::string key = trim(line.substr(0, equal_pos));
    std::string value = trim(line.substr(equal_pos + 1));
    // wrong line
    if (key.empty() || value.empty()) continue;
    curr.Set(key, value);
  }
  if (!curr.Empty()) {
    sections->emplace(curr.Name(), curr);
  }
  return true;
}
