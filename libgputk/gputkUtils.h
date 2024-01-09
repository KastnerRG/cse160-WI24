#ifndef __GPUTK_UTILS_H__
#define __GPUTK_UTILS_H__

#include "gputkString.h"
#include "vendor/sole.hpp"

#ifdef GPUTK_DEBUG
#define DEBUG(...) __VA_ARGS__
#else /* GPUTK_DEBUG */
#define DEBUG(...)
#endif /* GPUTK_DEBUG */

static char* uuid() {
  auto u4 = sole::uuid4();
  return gpuTKString_duplicate(u4.str());
}

#endif /* __GPUTK_UTILS_H__ */
