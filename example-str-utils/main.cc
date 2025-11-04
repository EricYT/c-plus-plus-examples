#include <cassert>
#include <fstream>
#include <iostream>

#include "section.h"
#include "string.h"

void TestTrimString() {
  assert("hello" == trim("hello"));
  assert("hello" == trim(" hello"));
  assert("hello" == trim("hello "));
  assert("hello" == trim(" hello "));
  assert("hello" == trim("         hello "));
  assert("hello" == trim("         hello             "));

  assert("hello" == trim("\thello             "));
  assert("hello" == trim("\thello          \t   "));
  assert("hello" == trim("\t \thello       \t   \t   "));
  assert("hello" == trim("\t \thello\t\t"));
}

void TestIniParser() {
  Section s("hello");
  s.Set("t0", "124");
  s.Set("t1", "-124");
  s.Set("t2", "124");

  assert(s.Get<int32_t>("t0", 0) == 124);
  assert(s.Get<int64_t>("t1", 0LL) == -124LL);
  assert(s.Get<uint32_t>("t2", 0U) == 124U);
  assert(s.Get<uint64_t>("t2", 0ULL) == 124ULL);

  s.Set("t3", "true");
  s.Set("t4", "TrUE");
  s.Set("t5", "False");
  s.Set("t6", "FALSE");
  s.Set("t7", "false");
  s.Set("t8", "false1");

  assert(s.Get<bool>("t3", false) == true);
  assert(s.Get<bool>("t4", false) == true);
  assert(s.Get<bool>("t5", true) == false);
  assert(s.Get<bool>("t6", true) == false);
  assert(s.Get<bool>("t7", true) == false);
  assert(s.Get<bool>("t8", true) == true);
  assert(s.Get<bool>("t9", true) == true);
  assert(s.Get<bool>("t9", false) == false);
}

const static std::string INI_FILE = "./test.ini";

void PrepareIniFile() {
  const std::string content = R"(    " this is a comment
[section0]
  key0 = value
   # this is a comment


key1      = 124

[section1]
         key0 = True


"
# this is comment
key1=
key2=hello world)";
  std::ofstream file(INI_FILE);
  assert(file.is_open());
  file << content;
  file.close();
}

void TestParseIniFile() {
  PrepareIniFile();

  Sections sections;
  bool ok = ParseSectionFile(INI_FILE, &sections);
  assert(ok);
  assert(sections.size() == 2);

  const Section& s0 = sections["section0"];
  assert(s0.Name() == "section0");
  assert(s0.Get<std::string>("key0", "") == "value");
  assert(s0.Get<int32_t>("key1", 0) == 124);
  assert(s0.Get<int32_t>("key2", 0) == 0);

  const Section& s1 = sections["section1"];
  assert(s1.Name() == "section1");
  assert(s1.Get<bool>("key0", false) == true);
  assert(s1.Get<int32_t>("key1", 0) == 0);
  assert(s1.Get<std::string>("key2", "") == "hello world");

  const Section& s2 = sections["section2"];
  assert(s2.Empty());
}

int main() {
  TestTrimString();
  TestIniParser();
  TestParseIniFile();
  return 0;
}
