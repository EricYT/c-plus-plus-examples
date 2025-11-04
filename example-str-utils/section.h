#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

class Section {
 public:
  Section() {}

  Section(const std::string& name) : name_(name) {}

  ~Section() = default;

  bool Empty() const { return name_.empty(); }

  void Clear() {
    name_.clear();
    data_.clear();
  }

  const std::string& Name() const { return name_; }

  void SetName(const std::string& name) { name_ = name; }

  void Set(const std::string& key, const std::string& value) {
    assert(!name_.empty());
    data_[key] = value;
  }

  template <typename T>
  T Get(const std::string& key, const T& def) const;

 private:
  std::string name_;
  std::unordered_map<std::string, std::string> data_;
};

using Sections = std::unordered_map<std::string, Section>;

template <>
int32_t Section::Get<int32_t>(const std::string& key, const int32_t& def) const;

template <>
int64_t Section::Get<int64_t>(const std::string& key, const int64_t& def) const;

template <>
uint32_t Section::Get<uint32_t>(const std::string& key,
                                const uint32_t& def) const;

template <>
uint64_t Section::Get<uint64_t>(const std::string& key,
                                const uint64_t& def) const;

template <>
bool Section::Get<bool>(const std::string& key, const bool& def) const;

template <>
std::string Section::Get<std::string>(const std::string& key,
                                      const std::string& def) const;

bool ParseSectionFile(const std::string& filename, Sections* sections);
