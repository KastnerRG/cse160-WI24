#include "gputk.h"

template <typename T>
static inline T _min(const T &x, const T &y) {
  return x < y ? x : y;
}

template <typename T>
static inline T _max(const T &x, const T &y) {
  return x > y ? x : y;
}

template <typename T>
inline T lerp(const double &x, const T &start, const T &end) {
  return (1 - x) * start + x * end;
}

static inline void genRandom(void *trgt, gpuTKType_t type, double minVal,
                             double maxVal) {
  const int span  = maxVal - minVal;
  const int r     = rand();
  const double rf = ((double)r) / ((double)RAND_MAX);
  switch (type) {
    case gpuTKType_ascii:
      *((char *)trgt) = (r % span) + minVal; // random printable character;
      break;
    case gpuTKType_bit8:
      *((char *)trgt) = lerp<char>(rf, minVal, maxVal);
      break;
    case gpuTKType_ubit8:
      *((unsigned char *)trgt) = lerp<unsigned char>(rf, minVal, maxVal);
      break;
    case gpuTKType_integer:
      *((int *)trgt) = lerp<int>(rf, minVal, maxVal);
      break;
    case gpuTKType_float: {
      *((float *)trgt) = lerp<float>(rf, minVal, maxVal);
      break;
    }
    case gpuTKType_double: {
      *((double *)trgt) = lerp<double>(rf, minVal, maxVal);
      break;
    }
    case gpuTKType_unknown:
      gpuTKAssert(false && "Invalid gpuTKType_unknown");
      break;
  }
  return;
}

static inline void *genRandomList(gpuTKType_t type, size_t len, double minVal,
                                  double maxVal) {
  size_t ii;
  void *data = gpuTKNewArray(char, gpuTKType_size(type) * len);
  switch (type) {
    case gpuTKType_ascii:
    case gpuTKType_bit8: {
      char *iter = (char *)data;
      for (ii = 0; ii < len; ii++) {
        genRandom(iter++, type, minVal, maxVal);
      }
      break;
    }
    case gpuTKType_ubit8: {
      unsigned char *iter = (unsigned char *)data;
      for (ii = 0; ii < len; ii++) {
        genRandom(iter++, type, minVal, maxVal);
      }
      break;
    }
    case gpuTKType_integer: {
      int *iter = (int *)data;
      for (ii = 0; ii < len; ii++) {
        genRandom(iter++, type, minVal, maxVal);
      }
      break;
    }
    case gpuTKType_float: {
      float *iter = (float *)data;
      for (ii = 0; ii < len; ii++) {
        genRandom(iter++, type, minVal, maxVal);
      }
      break;
    }
    case gpuTKType_double: {
      double *iter = (double *)data;
      for (ii = 0; ii < len; ii++) {
        genRandom(iter++, type, minVal, maxVal);
      }
      break;
    }
    case gpuTKType_unknown:
      gpuTKAssert(false && "Invalid gpuTKType_unknown");
      break;
  }
  return data;
}

static void genRaw(const char *path, gpuTKRaw_GenerateParams_t params) {
  int rows      = _max(1, params.rows);
  int cols      = _max(1, params.cols);
  double minVal = params.minVal;
  double maxVal = params.maxVal;
  gpuTKType_t type = params.type;
  void *data    = genRandomList(type, rows * cols, minVal, maxVal);
  gpuTKExport(path, gpuTKExportKind_raw, data, rows, cols, type);
  gpuTKDelete(data);
}

static void genCSV(const char *path, gpuTKCSV_GenerateParams_t params) {
  int rows      = _max(1, params.rows);
  int cols      = _max(1, params.cols);
  double minVal = params.minVal;
  double maxVal = params.maxVal;
  gpuTKType_t type = params.type;
  void *data    = genRandomList(type, rows * cols, minVal, maxVal);
  gpuTKExport(path, gpuTKExportKind_csv, data, rows, cols, type);
  gpuTKDelete(data);
}

static void genTSV(const char *path, gpuTKTSV_GenerateParams_t params) {
  int rows      = _max(1, params.rows);
  int cols      = _max(1, params.cols);
  double minVal = params.minVal;
  double maxVal = params.maxVal;
  gpuTKType_t type = params.type;
  void *data    = genRandomList(type, rows * cols, minVal, maxVal);
  gpuTKExport(path, gpuTKExportKind_tsv, data, rows, cols, type);
  gpuTKDelete(data);
}

static void genText(const char *path, gpuTKText_GenerateParams_t params) {
  int length    = _max(1, params.length);
  gpuTKType_t type = gpuTKType_ascii;
  void *data    = genRandomList(type, length, 32, 128);
  gpuTKExport(path, gpuTKExportKind_text, data, length, 1, type);
  gpuTKDelete(data);
}

static void genPPM(const char *path, gpuTKPPM_GenerateParams_t params) {
  int width     = _max(1, params.width);
  int height    = _max(1, params.height);
  int channels  = _max(1, params.channels);
  double minVal = params.minVal;
  double maxVal = params.maxVal;
  gpuTKType_t type = gpuTKType_float;
  float *data   = (float *)genRandomList(type, width * height * channels,
                                       minVal, maxVal);
  gpuTKImage_t img = gpuTKImage_new(width, height, channels, data);
  gpuTKExport(path, img);
  gpuTKImage_delete(img);
}

EXTERN_C void gpuTKDataset_generate(const char *path, gpuTKExportKind_t kind,
                                 gpuTKGenerateParams_t params) {
  gpuTKDirectory_create(gpuTKDirectory_name(path));

  switch (kind) {
    case gpuTKExportKind_raw:
      genRaw(path, params.raw);
      break;
    case gpuTKExportKind_csv:
      genCSV(path, params.csv);
      break;
    case gpuTKExportKind_tsv:
      genTSV(path, params.tsv);
      break;
    case gpuTKExportKind_ppm:
      genPPM(path, params.ppm);
      break;
    case gpuTKExportKind_text:
      genText(path, params.text);
      break;
    default:
      gpuTKAssert(false && "Invalid Export kind");
  }
}
