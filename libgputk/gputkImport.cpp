

#include "gputk.h"

static inline void gpuTKImportCSV_setFile(gpuTKImportCSV_t csv,
                                       const char *path) {
  if (csv != nullptr) {
    if (gpuTKImportCSV_getFile(csv) != nullptr) {
      gpuTKFile_delete(gpuTKImportCSV_getFile(csv));
    }
    if (path != nullptr) {
      gpuTKImportCSV_getFile(csv) = gpuTKFile_open(path, "r");
    } else {
      gpuTKImportCSV_getFile(csv) = nullptr;
    }
  }

  return;
}

static inline gpuTKImportCSV_t gpuTKImportCSV_new(void) {
  gpuTKImportCSV_t csv;

  csv = gpuTKNew(struct st_gpuTKImportCSV_t);

  gpuTKImportCSV_setRowCount(csv, -1);
  gpuTKImportCSV_setColumnCount(csv, -1);
  gpuTKImportCSV_setData(csv, NULL);
  gpuTKImportCSV_getFile(csv) = nullptr;
  gpuTKImportCSV_setSeperator(csv, '\0');

  return csv;
}

static inline void gpuTKImportCSV_delete(gpuTKImportCSV_t csv) {
  if (csv != nullptr) {
    gpuTKImportCSV_setFile(csv, NULL);
    if (gpuTKImportCSV_getData(csv)) {
      gpuTKDelete(gpuTKImportCSV_getData(csv));
    }
    gpuTKDelete(csv);
  }
}

static inline gpuTKImportCSV_t gpuTKImportCSV_findDimensions(gpuTKImportCSV_t csv,
                                                       int *resRows,
                                                       int *resColumns) {
  int rows = 0, columns = -1;
  char *line;
  gpuTKFile_t file;
  char seperator[2];

  if (csv == nullptr) {
    return NULL;
  }

  if (gpuTKImportCSV_getSeperator(csv) == '\0') {
    seperator[0] = ',';
  } else {
    seperator[0] = gpuTKImportCSV_getSeperator(csv);
  }
  seperator[1] = '\0';

  file = gpuTKImportCSV_getFile(csv);

  while ((line = gpuTKFile_readLine(file)) != nullptr) {
    int currColumn = 0;
    char *token    = strtok(line, seperator);
    while (token != nullptr) {
      token = strtok(NULL, seperator);
      currColumn++;
    }
    rows++;
    if (columns == -1) {
      columns = currColumn;
    }
    if (columns != currColumn) {
      gpuTKLog(ERROR, "The csv file is not rectangular.");
    }
    gpuTKAssert(columns == currColumn);
  }

  gpuTKFile_rewind(file);

  *resRows    = rows;
  *resColumns = columns;

  return csv;
}

static inline int *csv_readAsInteger(gpuTKFile_t file, char sep, int rows,
                                     int columns) {
  int ii = 0;
  int *data;
  char *line;
  int var;
  char seperator[2];

  if (file == nullptr) {
    return NULL;
  }

  data = gpuTKNewArray(int, rows *columns);

  if (sep == '\0') {
    seperator[0] = ',';
  } else {
    seperator[0] = sep;
  }
  seperator[1] = '\0';

  // printf("cols = %d rows = %d\n", columns, rows);
  if (columns == 1) {
    while ((line = gpuTKFile_readLine(file)) != nullptr) {
      sscanf(line, "%d", &var);
      // printf("reading %d\n", var);
      data[ii++] = var;
    }
  } else {
    while ((line = gpuTKFile_readLine(file)) != nullptr) {
      char *token = strtok(line, seperator);
      while (token != nullptr) {
        sscanf(token, "%d", &var);
        token      = strtok(NULL, seperator);
        data[ii++] = var;
      }
    }
  }

  return data;
}

static inline gpuTKReal_t *csv_readAsReal(gpuTKFile_t file, char sep, int rows,
                                       int columns) {
  int ii = 0;
  gpuTKReal_t *data;
  char *line;
  gpuTKReal_t var;
  char seperator[2];

  if (file == nullptr) {
    return NULL;
  }

  data = gpuTKNewArray(gpuTKReal_t, rows * columns);

  if (sep == '\0') {
    seperator[0] = ',';
  } else {
    seperator[0] = sep;
  }
  seperator[1] = '\0';

  if (columns == 1) {
    while ((line = gpuTKFile_readLine(file)) != nullptr) {
      sscanf(line, "%f", &var);
      data[ii++] = var;
    }
  } else {
    while ((line = gpuTKFile_readLine(file)) != nullptr) {
      char *token = strtok(line, seperator);
      while (token != nullptr) {
        sscanf(token, "%f", &var);
        token      = strtok(NULL, seperator);
        data[ii++] = var;
      }
    }
  }

  return data;
}

static inline gpuTKImportCSV_t gpuTKImportCSV_read(gpuTKImportCSV_t csv,
                                             gpuTKType_t type) {
  void *data;
  gpuTKFile_t file;
  char seperator;
  int rows, columns;

  if (csv == nullptr) {
    return NULL;
  }

  if (gpuTKImportCSV_getRowCount(csv) == -1 ||
      gpuTKImportCSV_getColumnCount(csv) == -1) {
    if (gpuTKImportCSV_findDimensions(csv, &rows, &columns) == nullptr) {
      gpuTKLog(ERROR, "Failed to figure out csv dimensions.");
      return NULL;
    }
    gpuTKImportCSV_setRowCount(csv, rows);
    gpuTKImportCSV_setColumnCount(csv, columns);
  }

  file      = gpuTKImportCSV_getFile(csv);
  seperator = gpuTKImportCSV_getSeperator(csv);
  rows      = gpuTKImportCSV_getRowCount(csv);
  columns   = gpuTKImportCSV_getColumnCount(csv);

  if (gpuTKImportCSV_getData(csv) != nullptr) {
    gpuTKDelete(gpuTKImportCSV_getData(csv));
    gpuTKImportCSV_setData(csv, NULL);
  }

  if (type == gpuTKType_integer) {
    // printf("ReadXXXing as integer...\n");
    data = csv_readAsInteger(file, seperator, rows, columns);
  } else {
    data = csv_readAsReal(file, seperator, rows, columns);
  }

  gpuTKImportCSV_setData(csv, data);

  return csv;
}

static inline gpuTKImportCSV_t gpuTKImportCSV_readAsInteger(gpuTKImportCSV_t csv) {
  return gpuTKImportCSV_read(csv, gpuTKType_integer);
}

static inline gpuTKImportCSV_t gpuTKImportCSV_readAsReal(gpuTKImportCSV_t csv) {
  return gpuTKImportCSV_read(csv, gpuTKType_real);
}

static inline void gpuTKImportRaw_setFile(gpuTKImportRaw_t raw,
                                       const char *path) {
  if (raw != nullptr) {
    if (gpuTKImportRaw_getFile(raw) != nullptr) {
      gpuTKFile_delete(gpuTKImportRaw_getFile(raw));
    }
    if (path != nullptr) {
      gpuTKImportRaw_getFile(raw) = gpuTKFile_open(path, "r");
    } else {
      gpuTKImportRaw_getFile(raw) = nullptr;
    }
  }

  return;
}

static inline gpuTKImportRaw_t gpuTKImportRaw_new(void) {
  gpuTKImportRaw_t raw;

  raw = gpuTKNew(struct st_gpuTKImportRaw_t);

  gpuTKImportRaw_setRowCount(raw, -1);
  gpuTKImportRaw_setColumnCount(raw, -1);
  gpuTKImportRaw_setData(raw, NULL);
  gpuTKImportRaw_getFile(raw) = nullptr;

  return raw;
}

static inline void gpuTKImportRaw_delete(gpuTKImportRaw_t raw) {
  if (raw != nullptr) {
    gpuTKImportRaw_setFile(raw, NULL);
    if (gpuTKImportRaw_getData(raw)) {
      gpuTKDelete(gpuTKImportRaw_getData(raw));
    }
    gpuTKDelete(raw);
  }
}

static inline gpuTKBool lineHasSpace(const char *line) {
  while (*line != '\0') {
    if (*line == ' ') {
      return gpuTKTrue;
    }
    line++;
  }
  return gpuTKFalse;
}

static inline char *lineStrip(const char *line) {
  char *sl    = gpuTKString_duplicate(line);
  char *iter  = sl;
  size_t slen = strlen(line);

  iter += slen - 1;
  while (*iter == '\0' || *iter == '\r' || *iter == '\t' ||
         *iter == '\n' || *iter == ' ') {
    *iter-- = '\0';
  }
  return sl;
}

static inline gpuTKBool gpuTKImportRaw_findDimensions(gpuTKImportRaw_t raw) {
  if (raw != nullptr) {
    int rows;
    int columns;
    char *line;
    gpuTKFile_t file;
    char *strippedLine;

    file = gpuTKImportRaw_getFile(raw);

    gpuTKFile_rewind(file);

    line = gpuTKFile_readLine(file);

    if (line == nullptr) {
      return gpuTKTrue;
    }

    strippedLine = lineStrip(line);

    if (lineHasSpace(strippedLine)) {
      sscanf(strippedLine, "%d %d", &rows, &columns);
    } else {
      columns = 1;
      sscanf(strippedLine, "%d", &rows);
    }

    gpuTKImportRaw_setRowCount(raw, rows);
    gpuTKImportRaw_setColumnCount(raw, columns);

    gpuTKDelete(strippedLine);

    return gpuTKFalse;
  }

  return gpuTKTrue;
}

static inline gpuTKImportRaw_t gpuTKImportRaw_read(gpuTKImportRaw_t raw,
                                             gpuTKType_t type) {
  void *data;
  gpuTKFile_t file;
  char seperator;
  int rows, columns;

  if (raw == nullptr) {
    return NULL;
  }

  if (gpuTKImportRaw_getRowCount(raw) == -1 ||
      gpuTKImportRaw_getColumnCount(raw) == -1) {
    if (gpuTKImportRaw_findDimensions(raw)) {
      gpuTKLog(ERROR, "Failed to figure out raw dimensions.");
      return NULL;
    }
  }

  file      = gpuTKImportRaw_getFile(raw);
  seperator = ' ';
  rows      = gpuTKImportRaw_getRowCount(raw);
  columns   = gpuTKImportRaw_getColumnCount(raw);

  if (gpuTKImportRaw_getData(raw) != nullptr) {
    gpuTKDelete(gpuTKImportRaw_getData(raw));
    gpuTKImportRaw_setData(raw, NULL);
  }

  if (type == gpuTKType_integer) {
    // printf("Rdin gas integer...\n");
    data = csv_readAsInteger(file, seperator, rows, columns);
  } else {
    data = csv_readAsReal(file, seperator, rows, columns);
  }

  gpuTKImportRaw_setData(raw, data);

  return raw;
}

static inline gpuTKImportRaw_t gpuTKImportRaw_readAsInteger(gpuTKImportRaw_t raw) {
  return gpuTKImportRaw_read(raw, gpuTKType_integer);
}

static inline gpuTKImportRaw_t gpuTKImportRaw_readAsReal(gpuTKImportRaw_t raw) {
  return gpuTKImportRaw_read(raw, gpuTKType_real);
}

static inline gpuTKImportText_t gpuTKImportText_new(void) {
  gpuTKImportText_t text;

  text = gpuTKNew(struct st_gpuTKImportText_t);

  gpuTKImportText_setLength(text, 0);
  gpuTKImportText_setData(text, NULL);
  gpuTKImportText_getFile(text) = nullptr;

  return text;
}

static inline void gpuTKImportText_setFile(gpuTKImportText_t text,
                                        const char *path) {
  if (text != nullptr) {
    if (gpuTKImportText_getFile(text) != nullptr) {
      gpuTKFile_delete(gpuTKImportText_getFile(text));
    }
    if (path != nullptr) {
      gpuTKImportText_getFile(text) = gpuTKFile_open(path, "r");
    } else {
      gpuTKImportText_getFile(text) = nullptr;
    }
  }

  return;
}

static inline void gpuTKImportText_delete(gpuTKImportText_t text) {
  if (text != nullptr) {
    gpuTKImportText_setFile(text, NULL);
    if (gpuTKImportText_getData(text)) {
      gpuTKDelete(gpuTKImportText_getData(text));
    }
    gpuTKDelete(text);
  }
}

static inline gpuTKImportText_t gpuTKImportText_read(gpuTKImportText_t text) {
  char *data;
  gpuTKFile_t file;
  int length;

  if (text == nullptr) {
    return NULL;
  }

  file = gpuTKImportText_getFile(text);

  if (gpuTKImportText_getData(text) != nullptr) {
    gpuTKDelete(gpuTKImportText_getData(text));
    gpuTKImportText_setData(text, NULL);
  }

  length = gpuTKFile_size(file);
  data   = gpuTKFile_read(file, length);

  gpuTKImportText_setData(text, data);
  gpuTKImportText_setLength(text, length);

  return text;
}

static inline gpuTKImport_t gpuTKImport_open(const char *file,
                                       gpuTKImportKind_t kind) {
  gpuTKImport_t imp;

  if (file == nullptr) {
    gpuTKLog(ERROR, "Go NULL for file value.");
    gpuTKExit();
  }

  if (!gpuTKFile_existsQ(file)) {
    gpuTKLog(ERROR, "File ", file, " does not exist.");
    gpuTKExit();
  }

  gpuTKImport_setKind(imp, kind);

  if (kind == gpuTKImportKind_raw) {
    gpuTKImportRaw_t raw = gpuTKImportRaw_new();
    gpuTKImportRaw_setFile(raw, file);
    gpuTKImport_setRaw(imp, raw);
  } else if (kind == gpuTKImportKind_tsv || kind == gpuTKImportKind_csv) {
    gpuTKImportCSV_t csv = gpuTKImportCSV_new();
    if (kind == gpuTKImportKind_csv) {
      gpuTKImportCSV_setSeperator(csv, ',');
    } else {
      gpuTKImportCSV_setSeperator(csv, '\t');
    }
    gpuTKImportCSV_setFile(csv, file);
    gpuTKImport_setCSV(imp, csv);
  } else if (kind == gpuTKImportKind_text) {
    gpuTKImportText_t text = gpuTKImportText_new();
    gpuTKImportText_setFile(text, file);
    gpuTKImport_setText(imp, text);
  } else if (kind == gpuTKImportKind_ppm) {
    gpuTKImage_t img = gpuTKPPM_import(file);
    gpuTKImport_setImage(imp, img);
  } else {
    gpuTKLog(ERROR, "Invalid import type.");
    gpuTKExit();
  }

  return imp;
}

static inline gpuTKImport_t gpuTKImport_open(const char *file,
                                       const char *type0) {
  gpuTKImport_t imp;
  gpuTKImportKind_t kind;
  char *type;

  type = gpuTKString_toLower(type0);

  if (gpuTKString_sameQ(type, "csv")) {
    kind = gpuTKImportKind_csv;
  } else if (gpuTKString_sameQ(type, "tsv")) {
    kind = gpuTKImportKind_tsv;
  } else if (gpuTKString_sameQ(type, "raw") || gpuTKString_sameQ(type, "dat")) {
    kind = gpuTKImportKind_raw;
  } else if (gpuTKString_sameQ(type, "ppm")) {
    kind = gpuTKImportKind_ppm;
  } else if (gpuTKString_sameQ(type, "text") || gpuTKString_sameQ(type, "txt")) {
    kind = gpuTKImportKind_text;
  } else {
    gpuTKLog(ERROR, "Invalid import type ", type0);
    gpuTKExit();
  }

  imp = gpuTKImport_open(file, kind);

  gpuTKDelete(type);

  return imp;
}

static inline void gpuTKImport_close(gpuTKImport_t imp) {
  gpuTKImportKind_t kind;

  kind = gpuTKImport_getKind(imp);
  if (kind == gpuTKImportKind_tsv || kind == gpuTKImportKind_csv) {
    gpuTKImportCSV_t csv = gpuTKImport_getCSV(imp);
    gpuTKImportCSV_delete(csv);
    gpuTKImport_setCSV(imp, NULL);
  } else if (kind == gpuTKImportKind_raw) {
    gpuTKImportRaw_t raw = gpuTKImport_getRaw(imp);
    gpuTKImportRaw_delete(raw);
    gpuTKImport_setRaw(imp, NULL);
  } else if (kind == gpuTKImportKind_text) {
    gpuTKImportText_t text = gpuTKImport_getText(imp);
    gpuTKImportText_delete(text);
    gpuTKImport_setText(imp, NULL);
  } else if (kind == gpuTKImportKind_ppm) {
  } else {
    gpuTKLog(ERROR, "Invalid import type.");
    gpuTKExit();
  }
  return;
}

static inline void *gpuTKImport_read(gpuTKImport_t imp, gpuTKType_t type) {
  void *data = nullptr;
  gpuTKImportKind_t kind;

  kind = gpuTKImport_getKind(imp);
  if (kind == gpuTKImportKind_tsv || kind == gpuTKImportKind_csv) {
    gpuTKImportCSV_t csv = gpuTKImport_getCSV(imp);
    gpuTKImportCSV_read(csv, type);
    data = gpuTKImportCSV_getData(csv);
  } else if (kind == gpuTKImportKind_raw) {
    gpuTKImportRaw_t raw = gpuTKImport_getRaw(imp);
    gpuTKImportRaw_read(raw, type);
    data = gpuTKImportRaw_getData(raw);
  } else if (gpuTKImportKind_text == kind) {
    gpuTKImportText_t text = gpuTKImport_getText(imp);
    text                = gpuTKImportText_read(text);
    data                = gpuTKImportText_getData(text);

  } else {
    gpuTKLog(ERROR, "Invalid import type.");
    gpuTKExit();
  }
  return data;
}

static inline int *gpuTKImport_readAsInteger(gpuTKImport_t imp) {
  void *data = gpuTKImport_read(imp, gpuTKType_integer);
  return (int *)data;
}

static inline gpuTKReal_t *gpuTKImport_readAsReal(gpuTKImport_t imp) {
  void *data = gpuTKImport_read(imp, gpuTKType_real);
  return (gpuTKReal_t *)data;
}

static inline gpuTKChar_t *gpuTKImport_readAsText(gpuTKImport_t imp) {
  void *data = gpuTKImport_read(imp, gpuTKType_ubit8);
  return (gpuTKChar_t *)data;
}

static gpuTKImportKind_t _parseImportExtension(const char *file) {
  char *extension;
  gpuTKImportKind_t kind;

  extension = gpuTKFile_extension(file);

  if (gpuTKString_sameQ(extension, "csv")) {
    kind = gpuTKImportKind_csv;
  } else if (gpuTKString_sameQ(extension, "tsv")) {
    kind = gpuTKImportKind_tsv;
  } else if (gpuTKString_sameQ(extension, "raw") ||
             gpuTKString_sameQ(extension, "dat")) {
    kind = gpuTKImportKind_raw;
  } else if (gpuTKString_sameQ(extension, "ppm") ||
             gpuTKString_sameQ(extension, "pbm")) {
    kind = gpuTKImportKind_ppm;
  } else if (gpuTKString_sameQ(extension, "text") ||
             gpuTKString_sameQ(extension, "txt")) {
    kind = gpuTKImportKind_text;
  } else {
    kind = gpuTKImportKind_unknown;
    gpuTKLog(ERROR, "File ", file, " does not have a compatible extension.");
  }

  gpuTKDelete(extension);

  return kind;
}

void *gpuTKImport(const char *file, int *resRows, int *resColumns,
               const char *type) {
  void *data, *res;
  gpuTKImport_t imp;
  size_t sz;
  int columns = 0, rows = 0;
  gpuTKImportKind_t kind;

  if (file == nullptr) {
    fprintf(stderr, "Failed to import file.\n");
    gpuTKExit();
  }

  kind = _parseImportExtension(file);

  gpuTKAssert(kind != gpuTKImportKind_unknown);

  imp = gpuTKImport_open(file, kind);
  if (gpuTKString_sameQ(type, "Real")) {
    data = gpuTKImport_readAsReal(imp);
    sz   = sizeof(gpuTKReal_t);
  } else if (gpuTKString_sameQ(type, "Text")) {
    data = gpuTKImport_readAsText(imp);
    sz   = sizeof(char);
  } else {
    // printf("Reading as integer..d\n");
    data = gpuTKImport_readAsInteger(imp);
    sz   = sizeof(int);
  }

  if (kind == gpuTKImportKind_csv || kind == gpuTKImportKind_tsv) {
    rows    = gpuTKImportCSV_getRowCount(gpuTKImport_getCSV(imp));
    columns = gpuTKImportCSV_getColumnCount(gpuTKImport_getCSV(imp));
  } else if (kind == gpuTKImportKind_raw) {
    rows    = gpuTKImportRaw_getRowCount(gpuTKImport_getRaw(imp));
    columns = gpuTKImportRaw_getColumnCount(gpuTKImport_getRaw(imp));
  } else if (kind == gpuTKImportKind_text) {
    rows    = 1;
    columns = gpuTKImportText_getLength(gpuTKImport_getText(imp));
  }

  if (rows == 1 && columns > 0) {
    rows    = columns;
    columns = 1;
  }

  if (resRows != nullptr) {
    *resRows = rows;
  }

  if (resColumns != nullptr) {
    *resColumns = columns;
  }

  res = gpuTKMalloc(sz * rows * columns);
  memcpy(res, data, sz * rows * columns);

  gpuTKImport_close(imp);

  return res;
}

void *gpuTKImport(const char *file, int *rows, int *columns) {
  return gpuTKImport(file, rows, columns, "Real");
}

EXTERN_C void *gpuTKImport(const char *file, int *rows) {
  return gpuTKImport(file, rows, NULL, "Real");
}

void *gpuTKImport(const char *file, int *res_rows, const char *type) {
  int cols, rows;
  void *res = gpuTKImport(file, &rows, &cols, type);
  if (rows == 1 && cols > 1) {
    rows = cols;
  }
  *res_rows = rows;
  return res;
}

gpuTKImage_t gpuTKImport(const char *file) {
  gpuTKImage_t img;
  gpuTKImport_t imp;
  gpuTKImportKind_t kind;

  if (file == nullptr) {
    fprintf(stderr, "Failed to import file.\n");
    gpuTKExit();
  }

  kind = _parseImportExtension(file);

  gpuTKAssert(kind == gpuTKImportKind_ppm);

  imp = gpuTKImport_open(file, kind);
  img = gpuTKImport_getImage(imp);
  gpuTKImport_close(imp);

  return img;
}

int gpuTKImport_flag(const char *file) {
  int res;
  gpuTKFile_t fh      = gpuTKFile_open(file, "r");
  const char *line = gpuTKFile_readLine(fh);
  sscanf(line, "%d", &res);
  gpuTKFile_close(fh);
  return res;
}
