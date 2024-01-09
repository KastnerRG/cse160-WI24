
#include "gputk.h"

char *solutionJSON = nullptr;
static string _solution_correctQ("");

static void _onUnsameImageFunction(string str) {
  _solution_correctQ = str;
}

template <typename T>
static gpuTKBool gpuTKSolution_listCorrectQ(const char *expectedOutputFile,
                                      gpuTKSolution_t sol, const char *type) {
  gpuTKBool res;
  T *expectedData;
  int expectedRows, expectedColumns;

  expectedData = (T *)gpuTKImport(expectedOutputFile, &expectedRows,
                               &expectedColumns, type);

  if (expectedData == nullptr) {
    _solution_correctQ = "Failed to open expected output file.";
    res                = gpuTKFalse;
  } else if (expectedRows != gpuTKSolution_getRows(sol)) {
    gpuTKLog(TRACE, "Number of rows in the solution is ",
          gpuTKSolution_getRows(sol), ". Expected number of rows is ",
          expectedRows, ".");
    _solution_correctQ =
        "The number of rows in the solution did not match "
        "that of the expected results.";
    res = gpuTKFalse;
  } else if (expectedColumns != gpuTKSolution_getColumns(sol)) {
    gpuTKLog(TRACE, "Number of columns in the solution is ",
          gpuTKSolution_getColumns(sol), ". Expected number of columns is ",
          expectedColumns, ".");
    _solution_correctQ = "The number of columns in the solution did not "
                         "match that of the expected results.";
    res = gpuTKFalse;
  } else {
    int ii, jj, idx;
    T *solutionData;

    solutionData = (T *)gpuTKSolution_getData(sol);
    if (gpuTKSolution_getType(sol) == "integral_vector/sorted") {
        gpuTKSort(solutionData, expectedRows * expectedColumns);
    }

    for (ii = 0; ii < expectedRows; ii++) {
      for (jj = 0; jj < expectedColumns; jj++) {
        idx = ii * expectedColumns + jj;
        if (gpuTKUnequalQ(expectedData[idx], solutionData[idx])) {
          string str;
          if (expectedColumns == 1) {
            str = gpuTKString(
                "The solution did not match the expected results at row ",
                ii, ". Expecting ", expectedData[idx], " but got ",
                solutionData[idx], ".");
          } else {
            str = gpuTKString("The solution did not match the expected "
                           "results at column ",
                           jj, " and row ", ii, ". Expecting ",
                           expectedData[idx], " but got ",
                           solutionData[idx], ".");
          }
          _solution_correctQ = str;
          res                = gpuTKFalse;
          goto matrixCleanup;
        }
      }
    }

    res = gpuTKTrue;
  matrixCleanup:
    if (expectedData != nullptr) {
      gpuTKFree(expectedData);
    }
  }
  return res;
}

static gpuTKBool gpuTKSolution_correctQ(char *expectedOutputFile,
                                  gpuTKSolution_t sol) {
  if (expectedOutputFile == nullptr) {
    _solution_correctQ = "Failed to determined the expected output file.";
    return gpuTKFalse;
  } else if (!gpuTKFile_existsQ(expectedOutputFile)) {
    _solution_correctQ =
        gpuTKString("The file ", expectedOutputFile, " does not exist.");
    return gpuTKFalse;
  } else if (gpuTKString_sameQ(gpuTKSolution_getType(sol), "image")) {
    gpuTKBool res;
    gpuTKImage_t solutionImage = nullptr;
    gpuTKImage_t expectedImage = gpuTKImport(expectedOutputFile);
    if (expectedImage == nullptr) {
      _solution_correctQ = "Failed to open expected output file.";
      res                = gpuTKFalse;
    } else if (gpuTKImage_getWidth(expectedImage) !=
               gpuTKSolution_getWidth(sol)) {
      _solution_correctQ =
          "The image width of the expected image does not "
          "match that of the solution.";
      res = gpuTKFalse;
    } else if (gpuTKImage_getHeight(expectedImage) !=
               gpuTKSolution_getHeight(sol)) {
      _solution_correctQ =
          "The image height of the expected image does not "
          "match that of the solution.";
      res = gpuTKFalse;
    } else if (gpuTKImage_getChannels(expectedImage) !=
               gpuTKSolution_getChannels(sol)) {
      _solution_correctQ =
          "The image channels of the expected image does not "
          "match that of the solution.";
      res = gpuTKFalse;
    } else {
      solutionImage = (gpuTKImage_t)gpuTKSolution_getData(sol);
      gpuTKAssert(solutionImage != nullptr);
      res = gpuTKImage_sameQ(solutionImage, expectedImage,
                          _onUnsameImageFunction);
    }
    if (expectedImage != nullptr) {
      gpuTKImage_delete(expectedImage);
    }
    return res;
  } else if (gpuTKString_sameQ(gpuTKSolution_getType(sol), "histogram")) {
    return gpuTKSolution_listCorrectQ<unsigned char>(expectedOutputFile, sol,
                                                  "Integer");
  } else if (gpuTKString_sameQ(gpuTKSolution_getType(sol), "integral_vector/sorted") ||
             gpuTKString_sameQ(gpuTKSolution_getType(sol), "integral_vector")) {
    return gpuTKSolution_listCorrectQ<int>(expectedOutputFile, sol,
                                        "Integer");
  } else if (gpuTKString_sameQ(gpuTKSolution_getType(sol), "vector") ||
             gpuTKString_sameQ(gpuTKSolution_getType(sol), "matrix")) {
    return gpuTKSolution_listCorrectQ<gpuTKReal_t>(expectedOutputFile, sol,
                                             "Real");
  } else {
    gpuTKAssert(gpuTKFalse);
    return gpuTKFalse;
  }
}

gpuTKBool gpuTKSolution(char *expectedOutputFile, char *outputFile, char *type0,
                  void *data, int rows, int columns, int depth) {
  char *type;
  gpuTKBool res;
  gpuTKSolution_t sol;

  if (expectedOutputFile == nullptr || data == nullptr || type0 == nullptr) {
    gpuTKLog(ERROR, "Failed to grade solution, %s %s %s", expectedOutputFile, data, type0);
    return gpuTKFalse;
  }

  type = gpuTKString_toLower(type0);

  if (_solution_correctQ != "") {
    _solution_correctQ = "";
  }

  gpuTKSolution_setOutputFile(sol, outputFile);
  gpuTKSolution_setId(sol, uuid());
  gpuTKSolution_setSessionId(sol, sessionId());
  gpuTKSolution_setType(sol, type);
  gpuTKSolution_setData(sol, data);
  gpuTKSolution_setRows(sol, rows);
  gpuTKSolution_setColumns(sol, columns);
  gpuTKSolution_setDepth(sol, depth);

  res = gpuTKSolution_correctQ(expectedOutputFile, sol);

  if (outputFile != nullptr) {
    if (gpuTKString_sameQ(type, "image")) {
      gpuTKImage_t inputImage = (gpuTKImage_t)data;
      gpuTKImage_t img        = gpuTKImage_new(gpuTKImage_getWidth(inputImage),
                                  gpuTKImage_getHeight(inputImage),
                                  gpuTKImage_getChannels(inputImage));
      memcpy(gpuTKImage_getData(img), gpuTKImage_getData(inputImage),
             rows * columns * depth * sizeof(gpuTKReal_t));
      gpuTKExport(outputFile, img);
      gpuTKImage_delete(img);
    } else if (gpuTKString_sameQ(type, "integral_vector/sort")) {
      gpuTKSort((int *)data, rows*columns);
      gpuTKExport(outputFile, (int *)data, rows, columns);
    } else if (gpuTKString_sameQ(type, "integral_vector")) {
      gpuTKExport(outputFile, (int *)data, rows, columns);
    } else if (gpuTKString_sameQ(type, "vector") ||
               gpuTKString_sameQ(type, "matrix")) {
      gpuTKExport(outputFile, (gpuTKReal_t *)data, rows, columns);
    } else if (gpuTKString_sameQ(type, "histogram")) {
      gpuTKExport(outputFile, (unsigned char *)data, rows, columns);
    } else if (gpuTKString_sameQ(type, "text")) {
      gpuTKExport_text(outputFile, (unsigned char *)data, rows * columns);
    }
  }

  gpuTKFree(type);

  return res;
}

gpuTKBool gpuTKSolution(char *expectedOutputFile, char *outputFile, char *type0,
                  void *data, int rows, int columns) {
  return gpuTKSolution(expectedOutputFile, outputFile, type0, data, rows,
                    columns, 1);
}

gpuTKBool gpuTKSolution(gpuTKArg_t arg, void *data, int rows, int columns,
                  int depth) {
  char *type;
  gpuTKBool res;
  char *expectedOutputFile;
  char *outputFile;
  stringstream ss;

  expectedOutputFile = gpuTKArg_getExpectedOutputFile(arg);
  outputFile         = gpuTKArg_getOutputFile(arg);
  type               = gpuTKArg_getType(arg);

  gpuTKAssert(type != nullptr);
  gpuTKAssert(expectedOutputFile != nullptr);
  gpuTKAssert(outputFile != nullptr);

  res = gpuTKSolution(expectedOutputFile, outputFile, type, data, rows,
                   columns, depth);

  if (GPUTK_USE_JSON11) {
    json11::Json json;
    if (res) {
#ifndef JSON_OUTPUT
      printf("The solution is correct\n");
#endif
      json = json11::Json::object{{"correctq", true},
                                  {"message", "The solution is correct"}};
    } else {
#ifndef JSON_OUTPUT
      printf("The solution is NOT correct\n");
#endif
      json = json11::Json::object{{"correctq", false},
                                  {"message", _solution_correctQ}};
    }

#ifdef gpuTKLogger_printOnLog
    if (gpuTKLogger_printOnLog) {
      json11::Json e =
          json11::Json::object{{"type", "solution"}, {"data", json}};
      std::cout << e.dump() << std::endl;
    }
#endif /* gpuTKLogger_printOnLog */

    solutionJSON = gpuTKString_duplicate(json.string_value());
  } else {
    if (res) {
#ifndef JSON_OUTPUT
      printf("Solution is correct\n");
#endif
      ss << "{\n";
      ss << gpuTKString_quote("correctq") << ": true,\n";
      ss << gpuTKString_quote("message") << ": "
         << gpuTKString_quote("Solution is correct.") << "\n";
      ss << "}";
    } else {
#ifndef JSON_OUTPUT
      printf("Solution is NOT correct\n");
#endif
      ss << "{\n";
      ss << gpuTKString_quote("correctq") << ": false,\n";
      ss << gpuTKString_quote("message") << ": "
         << gpuTKString_quote(_solution_correctQ) << "\n";
      ss << "}";
    }
    solutionJSON = gpuTKString_duplicate(ss.str());
  }

  return res;
}

gpuTKBool gpuTKSolution(gpuTKArg_t arg, void *data, int rows, int columns) {
  return gpuTKSolution(arg, data, rows, columns, 1);
}

EXTERN_C gpuTKBool gpuTKSolution(gpuTKArg_t arg, void *data, int rows) {
  return gpuTKSolution(arg, data, rows, 1);
}

gpuTKBool gpuTKSolution(gpuTKArg_t arg, gpuTKImage_t img) {
  return gpuTKSolution(arg, img, gpuTKImage_getHeight(img),
                    gpuTKImage_getWidth(img), gpuTKImage_getChannels(img));
}
