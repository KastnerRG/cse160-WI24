

#ifndef __GPUTK_STRING_H__
#define __GPUTK_STRING_H__

#include <string.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include "gputk.h"

using std::string;
using std::vector;
using std::stringstream;
using std::exception;

template <typename T>
static inline string gpuTKString(const T &x);

static inline void gpuTKString_replace(string &value, string const &search,
                                    string const &replace) {
  for (string::size_type next = value.find(search); next != string::npos;
       next                   = value.find(search, next)) {
    value.replace(next, search.length(), replace);
    next += replace.length();
  }
}

static inline string gpuTKString_quote(string str) {
  string s = str;
  gpuTKString_replace(s, "\\", "\\\\");
  s = "\"" + s + "\"";
  return s;
}

static inline string gpuTKString_quote(const char *str) {
  if (str == nullptr) {
    return gpuTKString_quote("");
  } else {
    return gpuTKString_quote(string(str));
  }
}

static inline char *gpuTKString_duplicate(const char *str) {
  if (str == nullptr) {
    return NULL;
  } else {
    char *newstr;
    size_t len = strlen(str);
    newstr     = gpuTKNewArray(char, len + 1);
    memcpy(newstr, str, len * sizeof(char));
    newstr[len] = '\0';
    return newstr;
  }
}

static inline char *gpuTKString_duplicate(std::string str) {
  return gpuTKString_duplicate(str.c_str());
}

static inline string gpuTKString(void) {
  string s = "";
  return s;
}

template <typename T>
static inline string gpuTKString(const T &x) {
  try {
    stringstream ss;
    ss << x;
    return ss.str();
  } catch (exception &e) {
    return string();
  }
}

template <>
inline string gpuTKString(const bool &x) {
  return x ? "True" : "False";
}

template <>
inline string gpuTKString(const vector<string> &x) {
  stringstream ss;
  ss << "{";
  for (size_t ii = 0; ii < x.size(); ii++) {
    ss << gpuTKString_quote(x[ii]);
    if (ii != x.size() - 1) {
      ss << ", ";
    }
  }
  ss << "}";

  return ss.str();
}

template <>
inline string gpuTKString(const vector<int> &x) {
  stringstream ss;
  ss << "{";
  for (size_t ii = 0; ii < x.size(); ii++) {
    ss << x[ii];
    if (ii != x.size() - 1) {
      ss << ", ";
    }
  }
  ss << "}";

  return ss.str();
}

template <>
inline string gpuTKString(const vector<double> &x) {
  stringstream ss;
  ss << "{";
  for (size_t ii = 0; ii < x.size(); ii++) {
    ss << x[ii];
    if (ii != x.size() - 1) {
      ss << ", ";
    }
  }
  ss << "}";

  return ss.str();
}

template <typename T0, typename T1>
static inline string gpuTKString(const T0 &x0, const T1 &x1) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1);

  return ss.str();
}

template <typename T0, typename T1, typename T2>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2);
  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7, const T8 &x8) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7, const T8 &x8,
                              const T9 &x9) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8) << gpuTKString(x9);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          typename T10>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7, const T8 &x8,
                              const T9 &x9, const T10 &x10) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8) << gpuTKString(x9) << gpuTKString(x10);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          typename T10, typename T11>
static inline string
gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2, const T3 &x3,
         const T4 &x4, const T5 &x5, const T6 &x6, const T7 &x7,
         const T8 &x8, const T9 &x9, const T10 &x10, const T11 &x11) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8) << gpuTKString(x9) << gpuTKString(x10) << gpuTKString(x11);

  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          typename T10, typename T11, typename T12>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7, const T8 &x8,
                              const T9 &x9, const T10 &x10, const T11 &x11,
                              const T12 &x12) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8) << gpuTKString(x9) << gpuTKString(x10) << gpuTKString(x11)
     << gpuTKString(x12);
  return ss.str();
}

template <typename T0, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8, typename T9,
          typename T10, typename T11, typename T12, typename T13>
static inline string gpuTKString(const T0 &x0, const T1 &x1, const T2 &x2,
                              const T3 &x3, const T4 &x4, const T5 &x5,
                              const T6 &x6, const T7 &x7, const T8 &x8,
                              const T9 &x9, const T10 &x10, const T11 &x11,
                              const T12 &x12, const T13 &x13) {
  stringstream ss;
  ss << gpuTKString(x0) << gpuTKString(x1) << gpuTKString(x2) << gpuTKString(x3)
     << gpuTKString(x4) << gpuTKString(x5) << gpuTKString(x6) << gpuTKString(x7)
     << gpuTKString(x8) << gpuTKString(x9) << gpuTKString(x10) << gpuTKString(x11)
     << gpuTKString(x12) << gpuTKString(x13);
  return ss.str();
}

template <typename X, typename Y>
static inline gpuTKBool gpuTKString_sameQ(const X &x, const Y &y) {
  string xs = gpuTKString(x);
  string ys = gpuTKString(y);
  return strcmp(xs.c_str(), ys.c_str()) == 0;
}

static inline gpuTKBool gpuTKString_sameQ(const string &x, const string &y) {
  return x.compare(y) == 0;
}

static inline char *gpuTKString_toLower(const char *str) {
  if (str == nullptr) {
    return NULL;
  } else {
    char *res, *iter;

    res = iter = gpuTKString_duplicate(str);
    while (*iter != '\0') {
      *iter++ = tolower(*str++);
    }
    return res;
  }
}

static inline gpuTKBool gpuTKString_startsWith(const char *str,
                                         const char *prefix) {
  while (*prefix != '\0') {
    if (*str == '\0' || *str != *prefix) {
      return gpuTKFalse;
    }
    str++;
    prefix++;
  }
  return gpuTKTrue;
}

#endif /* __GPUTK_STRING_H__ */
