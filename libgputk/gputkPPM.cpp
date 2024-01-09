
#include <math.h>
#include "gputk.h"

static inline float _min(float x, float y) {
  return x < y ? x : y;
}

static inline float _max(float x, float y) {
  return x > y ? x : y;
}

static inline float _clamp(float x, float start, float end) {
  return _min(_max(x, start), end);
}

static const char *skipSpaces(const char *line) {
  while (*line == ' ' || *line == '\t') {
    line++;
    if (*line == '\0') {
      break;
    }
  }
  return line;
}

static char nextNonSpaceChar(const char *line0) {
  const char *line = skipSpaces(line0);
  return *line;
}

static gpuTKBool isComment(const char *line) {
  char nextChar = nextNonSpaceChar(line);
  if (nextChar == '\0') {
    return gpuTKTrue;
  } else {
    return nextChar == '#';
  }
}

static void parseDimensions(const char *line0, int *width, int *height) {
  const char *line = skipSpaces(line0);
  sscanf(line, "%d %d", width, height);
}

static void parseDimensions(const char *line0, int *width, int *height,
                            int *channels) {
  const char *line = skipSpaces(line0);
  sscanf(line, "%d %d %d", width, height, channels);
}

static void parseDepth(const char *line0, int *depth) {
  const char *line = skipSpaces(line0);
  sscanf(line, "%d", depth);
}

static char *nextLine(gpuTKFile_t file) {
  char *line = nullptr;
  while ((line = gpuTKFile_readLine(file)) != nullptr) {
    if (!isComment(line)) {
      break;
    }
  }
  return line;
}

gpuTKImage_t gpuTKPPM_import(const char *filename) {
  gpuTKImage_t img;
  gpuTKFile_t file;
  char *header;
  char *line;
  int ii, jj, kk, channels;
  int width, height, depth;
  unsigned char *charData, *charIter;
  float *imgData, *floatIter;
  float scale;

  img = nullptr;

  file = gpuTKFile_open(filename, "rb");
  if (file == nullptr) {
    printf("Could not open %s\n", filename);
    goto cleanup;
  }

  header = gpuTKFile_readLine(file);
  if (header == nullptr) {
    printf("Could not read from %s\n", filename);
    goto cleanup;
  } else if (strcmp(header, "P6") != 0 && strcmp(header, "P6\n") != 0 &&
             strcmp(header, "P5") != 0 && strcmp(header, "P5\n") != 0 &&
             strcmp(header, "S6") != 0 && strcmp(header, "S6\n") != 0) {
    printf("Could not find magic number for %s\n", filename);
    goto cleanup;
  }

  // P5 are monochrome while P6/S6 are rgb
  // S6 needs to parse number of channels out of file
  if (strcmp(header, "P5") == 0 || strcmp(header, "P5\n") == 0) {
    channels = 1;
    line     = nextLine(file);
    parseDimensions(line, &width, &height);
  } else if (strcmp(header, "P6") == 0 || strcmp(header, "P6\n") == 0) {
    channels = 3;
    line     = nextLine(file);
    parseDimensions(line, &width, &height);
  } else {
    line = nextLine(file);
    parseDimensions(line, &width, &height, &channels);
  }

  // the line now contains the depth information
  line = nextLine(file);
  parseDepth(line, &depth);

  // the rest of the lines contain the data in binary format
  charData = (unsigned char *)gpuTKFile_read(
      file, width * channels * sizeof(unsigned char), height);

  img = gpuTKImage_new(width, height, channels);

  imgData = gpuTKImage_getData(img);

  charIter  = charData;
  floatIter = imgData;

  scale = 1.0f / ((float)depth);

  for (ii = 0; ii < height; ii++) {
    for (jj = 0; jj < width; jj++) {
      for (kk = 0; kk < channels; kk++) {
        *floatIter = ((float)*charIter) * scale;
        floatIter++;
        charIter++;
      }
    }
  }

#ifdef LAZY_FILE_LOAD
  gpuTKDelete(charData);
#endif

cleanup:
  gpuTKFile_close(file);
  return img;
}

void gpuTKPPM_export(const char *filename, gpuTKImage_t img) {
  int ii;
  int jj;
  int kk;
  int depth;
  int width;
  int height;
  int channels;
  gpuTKFile_t file;
  float *floatIter;
  unsigned char *charData;
  unsigned char *charIter;
  
  file = gpuTKFile_open(filename, "w+"); //file = gpuTKFile_open(filename, "gpuTK+");

  width    = gpuTKImage_getWidth(img);
  height   = gpuTKImage_getHeight(img);
  channels = gpuTKImage_getChannels(img);
  depth    = 255;

  if (channels == 1) {
    gpuTKFile_writeLine(file, "P5");
  } else {
    gpuTKFile_writeLine(file, "P6");
  }
  gpuTKFile_writeLine(file, "#Created via gpuTKPPM Export");
  gpuTKFile_writeLine(file, gpuTKString(width, " ", height));
  gpuTKFile_writeLine(file, gpuTKString(depth));

  charData = gpuTKNewArray(unsigned char, width *height *channels);

  charIter  = charData;
  floatIter = gpuTKImage_getData(img);

  for (ii = 0; ii < height; ii++) {
    for (jj = 0; jj < width; jj++) {
      for (kk = 0; kk < channels; kk++) {
        *charIter = (unsigned char)ceil(_clamp(*floatIter, 0, 1) * depth);
        floatIter++;
        charIter++;
      }
    }
  }

  gpuTKFile_write(file, charData, width * channels * sizeof(unsigned char),
               height);

  gpuTKDelete(charData);
  gpuTKFile_delete(file);

  return;
}
