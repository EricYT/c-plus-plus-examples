#pragma once

#include <string>

using string = std::string;

struct Options {
// declare options name
#define OPTIONS_int32_t(name) int32_t name;
#define OPTIONS_int64_t(name) int64_t name;
#define OPTIONS_uint32_t(name) uint64_t name;
#define OPTIONS_uint64_t(name) uint32_t name;
#define OPTIONS_string(name) std::string name;
#define OPTIONS(name, type, defval, desc) OPTIONS_##type(name)
#include "options.inc"
#undef OPTIONS_int32_t
#undef OPTIONS_int64_t
#undef OPTIONS_uint32_t
#undef OPTIONS_uint64_t
#undef OPTIONS_string
#undef OPTIONS

  // construction with default value
  Options() {
#define OPTIONS_SET_DEF_VALUE(name, defval) name = defval;
#define OPTIONS(name, type, defval, desc) OPTIONS_SET_DEF_VALUE(name, defval)
#include "options.inc"
#undef OPTIONS_SET_DEF_VALUE
#undef OPTIONS
  }

// set_XXX methods, but it's unnecessary cause we can options.XXX = value;
#define OPTIONS_SET_VALUE(name, type) \
  void set_##name(type val) { name = val; }
#define OPTIONS(name, type, defval, desc) OPTIONS_SET_VALUE(name, type)
#include "options.inc"
#undef OPTIONS_SET_VALUE
#undef OPTIONS
};
