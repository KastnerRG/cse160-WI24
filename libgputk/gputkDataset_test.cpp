
#include "gputk.h"
#include "vendor/catch.hpp"

TEST_CASE("Can create Raw dataset", "[DataGenerator]") {
  gpuTKGenerateParams_t params;
  params.raw.rows   = 2;
  params.raw.cols   = 300;
  params.raw.minVal = 0;
  params.raw.maxVal = 30;
  params.raw.type   = gpuTKType_integer;
  gpuTKDataset_generate(
      gpuTKPath_join(gpuTKDirectory_current(), "test-dataset", "test.raw"),
      gpuTKExportKind_raw, params);
}

TEST_CASE("Can create Text dataset", "[DataGenerator]") {
  gpuTKGenerateParams_t params;
  params.text.length = 2000;
  gpuTKDataset_generate(
      gpuTKPath_join(gpuTKDirectory_current(), "test-dataset", "test.txt"),
      gpuTKExportKind_text, params);
}
