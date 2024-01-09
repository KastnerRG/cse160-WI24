

#ifndef __GPUTK_TYPES_H__
#define __GPUTK_TYPES_H__

#include "gputkAssert.h"

typedef bool gpuTKBool;
typedef float gpuTKReal_t;
typedef char gpuTKChar_t;

typedef struct st_gpuTKTimerNode_t *gpuTKTimerNode_t;
typedef struct st_gpuTKTimer_t *gpuTKTimer_t;
typedef struct st_gpuTKLogEntry_t *gpuTKLogEntry_t;
typedef struct st_gpuTKLogger_t *gpuTKLogger_t;
typedef struct st_gpuTKArg_t gpuTKArg_t;
typedef struct st_gpuTKImage_t *gpuTKImage_t;
typedef struct st_gpuTKFile_t *gpuTKFile_t;

#define gpuTKTrue true
#define gpuTKFalse false

typedef enum en_gpuTKType_t {
  gpuTKType_unknown = -1,
  gpuTKType_ascii   = 1,
  gpuTKType_bit8,
  gpuTKType_ubit8,
  gpuTKType_integer,
  gpuTKType_float,
  gpuTKType_double
} gpuTKType_t;

static inline size_t gpuTKType_size(gpuTKType_t ty) {
  switch (ty) {
    case gpuTKType_unknown:
      gpuTKAssert(false && "Invalid gpuTKType_unknown");
      return 0;
    case gpuTKType_ascii:
      return sizeof(char);
    case gpuTKType_bit8:
      return sizeof(char);
    case gpuTKType_ubit8:
      return sizeof(unsigned char);
    case gpuTKType_integer:
      return sizeof(int);
    case gpuTKType_float:
      return sizeof(float);
    case gpuTKType_double:
      return sizeof(double);
  }
  gpuTKAssert(false && "Invalid type");
  return 0;
}

#endif /* __GPUTK_TYPES_H__ */
