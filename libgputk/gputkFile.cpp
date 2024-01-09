

#include "gputk.h"

#define gpuTKFile_maxCount 256

static gpuTKFile_t gpuTKFile_handles[gpuTKFile_maxCount];

static int gpuTKFile_nextIndex(void) {
  int ii;
  for (ii = 0; ii < gpuTKFile_maxCount; ii++) {
    if (gpuTKFile_handles[ii] == nullptr) {
      return ii;
    }
  }
  gpuTKLog(ERROR, "Ran out of file handles.");
  gpuTKExit();
  return -1;
}

gpuTKFile_t gpuTKFile_new(void) {
  int idx       = gpuTKFile_nextIndex();
  gpuTKFile_t file = gpuTKNew(struct st_gpuTKFile_t);

  gpuTKAssert(idx >= 0);

  gpuTKFile_setIndex(file, idx);
  gpuTKFile_setFileName(file, NULL);
  gpuTKFile_setMode(file, NULL);
  gpuTKFile_setFileHandle(file, NULL);
  gpuTKFile_setData(file, NULL);

  gpuTKFile_handles[idx] = file;

  return file;
}

void gpuTKFile_delete(gpuTKFile_t file) {
  if (file != nullptr) {
    int idx = gpuTKFile_getIndex(file);
    if (gpuTKFile_getFileName(file) != nullptr) {
      gpuTKDelete(gpuTKFile_getFileName(file));
    }
    if (gpuTKFile_getMode(file) != nullptr) {
      gpuTKDelete(gpuTKFile_getMode(file));
    }
    if (gpuTKFile_getFileHandle(file) != nullptr) {
      fflush(gpuTKFile_getFileHandle(file));
      fclose(gpuTKFile_getFileHandle(file));
    }
    if (idx >= 0) {
      gpuTKAssert(gpuTKFile_handles[idx] == file);
      gpuTKFile_handles[idx] = nullptr;
    }
    if (gpuTKFile_getData(file) != nullptr) {
      gpuTKDelete(gpuTKFile_getData(file));
    }
    gpuTKDelete(file);
  }
}

void gpuTKFile_init(void) {
  int ii;

  for (ii = 0; ii < gpuTKFile_maxCount; ii++) {
    gpuTKFile_handles[ii] = nullptr;
  }
}

void gpuTKFile_atExit(void) {
  int ii;

  for (ii = 0; ii < gpuTKFile_maxCount; ii++) {
    if (gpuTKFile_handles[ii] != nullptr) {
      gpuTKFile_delete(gpuTKFile_handles[ii]);
    }
  }
}

int gpuTKFile_count(void) {
  int ii, count = 0;

  for (ii = 0; ii < gpuTKFile_maxCount; ii++) {
    if (gpuTKFile_handles[ii] != nullptr) {
      count++;
    }
  }
  return count;
}

gpuTKFile_t gpuTKFile_open(const char *fileName, const char *mode) {
  FILE *handle;
  gpuTKFile_t file;

  if (fileName == nullptr) {
    return NULL;
  }

  handle = fopen(fileName, mode);
  if (handle == nullptr) {
    gpuTKLog(ERROR, "Failed to open ", file, " in mode ", mode);
    return NULL;
  }

  file = gpuTKFile_new();
  gpuTKFile_setFileName(file, gpuTKString_duplicate(fileName));
  gpuTKFile_setMode(file, gpuTKString_duplicate(mode));
  gpuTKFile_setFileHandle(file, handle);

  return file;
}

gpuTKFile_t gpuTKFile_open(const char *fileName) {
  return gpuTKFile_open(fileName, "r");
}

void gpuTKFile_close(gpuTKFile_t file) {
  gpuTKFile_delete(file);
}

char *gpuTKFile_read(gpuTKFile_t file, size_t size, size_t count) {
  size_t res;
  char *buffer;
  size_t bufferLen;
  FILE *handle;

  if (file == nullptr) {
    return NULL;
  }
#ifndef LAZY_FILE_LOAD
  if (gpuTKFile_getData(file) != nullptr) {
    char *data = gpuTKFile_getData(file) + gpuTKFile_getDataOffset(file);
    gpuTKFile_setDataOffset(file, gpuTKFile_getDataOffset(file) + size * count);
    return data;
  }
#endif /* LAZY_FILE_LOAD */

  handle    = gpuTKFile_getFileHandle(file);
  bufferLen = size * count + 1;
  buffer    = gpuTKNewArray(char, bufferLen);

  res = fread(buffer, size, count, handle);
  // make valid C string
  buffer[size * res] = '\0';

  return buffer;
}

char *gpuTKFile_read(gpuTKFile_t file, size_t len) {
  char *buffer = gpuTKFile_read(file, sizeof(char), len);
  return buffer;
}

void gpuTKFile_rewind(gpuTKFile_t file) {
  if (file == nullptr) {
    return;
  }

  if (gpuTKFile_getData(file) == nullptr) {
    FILE *handle;
    handle = gpuTKFile_getFileHandle(file);
    gpuTKAssert(handle != nullptr);
    rewind(handle);
  }
#ifndef LAZY_FILE_LOAD
  else {
    gpuTKFile_setDataOffset(file, 0);
  }
#endif

  return;
}

size_t gpuTKFile_size(gpuTKFile_t file) {
  size_t len;
  FILE *handle;

  if (file == nullptr) {
    return 0;
  }
#ifndef LAZY_FILE_LOAD
  if (gpuTKFile_getData(file) != nullptr) {
    if (gpuTKFile_getLength(file) == 0) {
      gpuTKFile_setLength(file, strlen(gpuTKFile_getData(file)));
    }
    return gpuTKFile_getLength(file);
  }
#endif /* LAZY_FILE_LOAD */

  handle = gpuTKFile_getFileHandle(file);

  fseek(handle, 0, SEEK_END);
  len = ftell(handle);
  rewind(handle);

  return len;
}

char *gpuTKFile_read(gpuTKFile_t file) {
  size_t len;

  if (file == nullptr) {
    return NULL;
  }

  len = gpuTKFile_size(file);

  if (len == 0) {
    return NULL;
  }

  gpuTKFile_setLength(file, len);

  return gpuTKFile_read(file, len);
}

#define MAX_CHARS_PER_LINE (1 << 17)

static char buffer[MAX_CHARS_PER_LINE];

char *gpuTKFile_readLine(gpuTKFile_t file) {
  if (file == nullptr) {
    return NULL;
  }
#ifdef LAZY_FILE_LOAD
  FILE *handle;
  memset(buffer, 0, MAX_CHARS_PER_LINE);

  handle = gpuTKFile_getFileHandle(file);

  if (fgets(buffer, MAX_CHARS_PER_LINE - 1, handle)) {
    return buffer;
  } else {
    // gpuTKLog(ERROR, "Was not able to read line from ",
    // gpuTKFile_getFileName(file));
    return NULL;
  }
#else
  size_t newOffset;
  size_t lenToNewLine = 0;
  const char *tmp;

  if (gpuTKFile_getData(file) == nullptr) {
    gpuTKFile_setData(file, gpuTKFile_read(file));
    fclose(gpuTKFile_getFileHandle(file));
    gpuTKFile_setFileHandle(file, NULL);
    gpuTKFile_setDataOffset(file, 0);
    gpuTKFile_setLength(file, strlen(gpuTKFile_getData(file)));
  }

  memset(buffer, 0, MAX_CHARS_PER_LINE);

  if (gpuTKFile_getDataOffset(file) >= gpuTKFile_getLength(file)) {
    return NULL;
  }

  newOffset = gpuTKFile_getDataOffset(file);
  tmp       = gpuTKFile_getData(file) + gpuTKFile_getDataOffset(file);
  while (newOffset < gpuTKFile_getLength(file) && *tmp != '\n') {
    tmp++;
    lenToNewLine++;
    newOffset++;
  }

  memcpy(buffer, gpuTKFile_getData(file) + gpuTKFile_getDataOffset(file),
         lenToNewLine);
  gpuTKFile_setDataOffset(file, newOffset + 1);

  return buffer;
#endif
}

void gpuTKFile_write(gpuTKFile_t file, const void *buffer, size_t size,
                  size_t count) {
  size_t res;
  FILE *handle;

  if (file == nullptr) {
    return;
  }

  handle = gpuTKFile_getFileHandle(file);

  res = fwrite(buffer, size, count, handle);
  if (res != count) {
    gpuTKLog(ERROR, "Failed to write data to ", gpuTKFile_getFileName(file));
  }

  return;
}

void gpuTKFile_write(gpuTKFile_t file, const void *buffer, size_t len) {
  gpuTKFile_write(file, buffer, sizeof(char), len);
  return;
}

void gpuTKFile_write(gpuTKFile_t file, const char *buffer) {
  size_t len;

  len = strlen(buffer);
  gpuTKFile_write(file, buffer, len);

  return;
}

void gpuTKFile_writeLine(gpuTKFile_t file, const char *buffer0) {
  string buffer = gpuTKString(buffer0, "\n");
  gpuTKFile_write(file, buffer.c_str());
}

void gpuTKFile_write(gpuTKFile_t file, string buffer) {
  gpuTKFile_write(file, buffer.c_str());
}

void gpuTKFile_writeLine(gpuTKFile_t file, string buffer0) {
  string buffer = buffer0 + "\n";
  gpuTKFile_write(file, buffer.c_str());
}

gpuTKBool gpuTKFile_existsQ(const char *path) {
  if (path == nullptr) {
    return gpuTKFalse;
  } else {
    FILE *file = fopen(path, "r");
    if (file != nullptr) {
      fclose(file);
      return gpuTKTrue;
    }
    return gpuTKFalse;
  }
}

char *gpuTKFile_extension(const char *file) {
  char *extension;
  char *extensionLower;
  char *end;
  size_t len;

  len = strlen(file);
  end = (char *)&file[len - 1];
  while (*end != '.') {
    end--;
  }
  if (*end == '.') {
    end++;
  }

  extension      = gpuTKString_duplicate(end);
  extensionLower = gpuTKString_toLower(extension);
  gpuTKDelete(extension);

  return extensionLower;
}
