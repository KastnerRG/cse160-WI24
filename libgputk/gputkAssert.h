

#ifndef __GPUTK_ASSERT_H__
#define __GPUTK_ASSERT_H__

#include <assert.h>

#ifdef GPUTK_DEBUG
#define gpuTKAssert(cond) assert(cond)
#define gpuTKAssertMessage(msg, cond)                                        \
  do {                                                                    \
    if (!(cond)) {                                                        \
      gpuTKPrint(msg);                                                       \
      gpuTKAssert(cond);                                                     \
    }                                                                     \
  } while (0)
#else /* GPUTK_DEBUG */
#define gpuTKAssert(...)
#define gpuTKAssertMessage(...)
#endif /* GPUTK_DEBUG */

#define gpuTKTodo(msg) gpuTKAssertMessage(msg, false)

#endif /* __GPUTK_ASSERT_H__ */
