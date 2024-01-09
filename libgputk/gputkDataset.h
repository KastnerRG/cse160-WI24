#ifndef __GPUTK_DATASET_H__
#define __GPUTK_DATASET_H__

#include "gputkImport.h"
#include "gputkTypes.h"

typedef struct {
  int rows;
  int cols;
  gpuTKType_t type;
  double minVal;
  double maxVal;
} gpuTKCSV_GenerateParams_t;

typedef struct {
  int rows;
  int cols;
  gpuTKType_t type;
  double minVal;
  double maxVal;
} gpuTKTSV_GenerateParams_t;

typedef struct {
  int rows;
  int cols;
  double minVal;
  double maxVal;
  gpuTKType_t type;
} gpuTKRaw_GenerateParams_t;

typedef struct {
  int width;
  int height;
  int channels;
  double minVal;
  double maxVal;
} gpuTKPPM_GenerateParams_t;

typedef struct { int length; } gpuTKText_GenerateParams_t;

typedef union {
  gpuTKCSV_GenerateParams_t csv;
  gpuTKRaw_GenerateParams_t raw;
  gpuTKTSV_GenerateParams_t tsv;
  gpuTKPPM_GenerateParams_t ppm;
  gpuTKText_GenerateParams_t text;
} gpuTKGenerateParams_t;

EXTERN_C void gpuTKDataset_generate(const char *path, gpuTKExportKind_t kind,
                                 gpuTKGenerateParams_t params);

#endif /* __GPUTK_DATASET_H__ */
