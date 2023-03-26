#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @object: intro for object
 *
 *  @brief: basic header for lspe
 *
 *  @NOTES:
 *******************************/

#include <cassert>
#include <cmath>
#include <algorithm>
#include <float.h>
#include <stdint.h>

#ifdef DEBUG
#include <cstdio>
#include <ctime>
#define LSPE_ASSERT(e) assert(e)
#define LSPE_DEBUG(format, args...)                        \
 do {                                                      \
  time_t tm;                                               \
  time(&tm);                                               \
  struct tm *now = localtime(&tm);                         \
  fprintf(                                                 \
      stderr,                                              \
      "%d-%02d-%02d %02d:%02d:%02d - [INFO] " format "\n", \
      now->tm_year + 1900,                                 \
      now->tm_mon,                                         \
      now->tm_mday,                                        \
      now->tm_hour,                                        \
      now->tm_min,                                         \
      now->tm_sec,                                         \
      ##args);                                             \
 } while (0);
#else
#define LSPE_ASSERT(...)
#define LSPE_DEBUG(...)
#endif

#define LSPE_ALWAYS_ASSERT(e) assert(e)

namespace lspe {

const float Pi  = 3.1415926535897932384626433832795;
const float Inf = FLT_MAX;

template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T, typename... G>
T max(T first, G... args) {
    static_assert(sizeof...(args) >= 1, "lspe::max expects at least 2 params.");
    return max(first, max(args...));
}

template <typename T, typename... G>
T min(T first, G... args) {
    static_assert(sizeof...(args) >= 1, "lspe::min expects at least 2 params.");
    return min(first, min(args...));
}

}; // namespace lspe
