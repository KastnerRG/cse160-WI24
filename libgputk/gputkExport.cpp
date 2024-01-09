
#include "gputk.h"

static inline void gpuTKExportText_setFile(gpuTKExportText_t text,
                                        const char *path) {
  if (text != nullptr) {
    if (gpuTKExportText_getFile(text) != nullptr) {
      gpuTKFile_delete(gpuTKExportText_getFile(text));
    }
    if (path != nullptr) {
      gpuTKExportText_getFile(text) = gpuTKFile_open(path, "w+");
    } else {
      gpuTKExportText_getFile(text) = nullptr;
    }
  }

  return;
}

static inline gpuTKExportText_t gpuTKExportText_new(void) {
  gpuTKExportText_t text;

  text = gpuTKNew(struct st_gpuTKExportText_t);

  gpuTKExportText_getFile(text) = nullptr;
  gpuTKExportText_setLength(text, -1);

  return text;
}

static inline void gpuTKExportText_delete(gpuTKExportText_t text) {
  if (text != nullptr) {
    gpuTKExportText_setFile(text, NULL);
    gpuTKDelete(text);
  }
  return;
}

static inline void gpuTKExportText_write(gpuTKExportText_t text,
                                      const char *data, int length) {
  int ii;
  FILE *handle;
  gpuTKFile_t file;

  if (text == nullptr || gpuTKExportText_getFile(text) == nullptr) {
    return;
  }

  file = gpuTKExportText_getFile(text);

  handle = gpuTKFile_getFileHandle(file);

  if (handle == nullptr) {
    return;
  }

  for (ii = 0; ii < length; ii++) {
    fprintf(handle, "%c", data[ii]);
  }

  return;
}

static inline void gpuTKExportRaw_setFile(gpuTKExportRaw_t raw,
                                       const char *path) {
  if (raw != nullptr) {
    if (gpuTKExportRaw_getFile(raw) != nullptr) {
      gpuTKFile_delete(gpuTKExportRaw_getFile(raw));
    }
    if (path != nullptr) {
      gpuTKExportRaw_getFile(raw) = gpuTKFile_open(path, "w+");
    } else {
      gpuTKExportRaw_getFile(raw) = nullptr;
    }
  }

  return;
}

static inline gpuTKExportRaw_t gpuTKExportRaw_new(void) {
  gpuTKExportRaw_t raw;

  raw = gpuTKNew(struct st_gpuTKExportRaw_t);

  gpuTKExportRaw_getFile(raw) = nullptr;
  gpuTKExportRaw_setRowCount(raw, -1);
  gpuTKExportRaw_setColumnCount(raw, -1);

  return raw;
}

static inline void gpuTKExportRaw_delete(gpuTKExportRaw_t raw) {
  if (raw != nullptr) {
    gpuTKExportRaw_setFile(raw, NULL);
    gpuTKDelete(raw);
  }
  return;
}

static inline void gpuTKExportRaw_write(gpuTKExportRaw_t raw, void *data,
                                     int rows, int columns,
                                     gpuTKType_t type) {
  int ii, jj;
  FILE *handle;
  gpuTKFile_t file;

  if (raw == nullptr || gpuTKExportRaw_getFile(raw) == nullptr) {
    return;
  }

  file = gpuTKExportRaw_getFile(raw);

  handle = gpuTKFile_getFileHandle(file);

  if (handle == nullptr) {
    return;
  }

  if (columns == 1) {
    fprintf(handle, "%d\n", rows);
  } else {
    fprintf(handle, "%d %d\n", rows, columns);
  }

  for (ii = 0; ii < rows; ii++) {
    for (jj = 0; jj < columns; jj++) {
      if (type == gpuTKType_integer) {
        int elem = ((int *)data)[ii * columns + jj];
        fprintf(handle, "%d", elem);
      } else if (type == gpuTKType_ubit8) {
        int elem = ((unsigned char *)data)[ii * columns + jj];
        fprintf(handle, "%d", elem);
      } else {
        gpuTKReal_t elem = ((gpuTKReal_t *)data)[ii * columns + jj];
        fprintf(handle, "%f", elem);
      }
      if (jj == columns - 1) {
        fprintf(handle, "\n");
      } else {
        fprintf(handle, " ");
      }
    }
  }

  return;
}

static inline void gpuTKExportCSV_setFile(gpuTKExportCSV_t csv,
                                       const char *path) {
  if (csv != nullptr) {
    if (gpuTKExportCSV_getFile(csv) != nullptr) {
      gpuTKFile_delete(gpuTKExportCSV_getFile(csv));
    }
    if (path != nullptr) {
      gpuTKExportCSV_getFile(csv) = gpuTKFile_open(path, "w+");
    } else {
      gpuTKExportCSV_getFile(csv) = nullptr;
    }
  }

  return;
}

static inline gpuTKExportCSV_t gpuTKExportCSV_new(void) {
  gpuTKExportCSV_t csv;

  csv = gpuTKNew(struct st_gpuTKExportCSV_t);

  gpuTKExportCSV_getFile(csv) = nullptr;
  gpuTKExportCSV_setColumnCount(csv, -1);
  gpuTKExportCSV_setRowCount(csv, -1);
  gpuTKExportCSV_setSeperator(csv, '\0');

  return csv;
}

static inline void gpuTKExportCSV_delete(gpuTKExportCSV_t csv) {
  if (csv != nullptr) {
    gpuTKExportCSV_setFile(csv, NULL);
    gpuTKDelete(csv);
  }
}

static inline void gpuTKExportCSV_write(gpuTKExportCSV_t csv, void *data,
                                     int rows, int columns, char sep,
                                     gpuTKType_t type) {
  int ii, jj;
  gpuTKFile_t file;
  FILE *handle;
  char seperator[2];

  if (csv == nullptr || gpuTKExportCSV_getFile(csv) == nullptr) {
    return;
  }

  file = gpuTKExportCSV_getFile(csv);

  handle = gpuTKFile_getFileHandle(file);

  if (handle == nullptr) {
    return;
  }

  if (sep == '\0') {
    seperator[0] = ',';
  } else {
    seperator[0] = sep;
  }
  seperator[1] = '\0';

  for (ii = 0; ii < rows; ii++) {
    for (jj = 0; jj < columns; jj++) {
      if (type == gpuTKType_integer) {
        int elem = ((int *)data)[ii * columns + jj];
        fprintf(handle, "%d", elem);
      } else if (type == gpuTKType_ubit8) {
        int elem = ((unsigned char *)data)[ii * columns + jj];
        fprintf(handle, "%d", elem);
      } else {
        gpuTKReal_t elem = ((gpuTKReal_t *)data)[ii * columns + jj];
        fprintf(handle, "%f", elem);
      }
      if (jj == columns - 1) {
        fprintf(handle, "\n");
      } else {
        fprintf(handle, "%s", seperator);
      }
    }
  }

  return;
}

static inline gpuTKExport_t gpuTKExport_open(const char *file,
                                       gpuTKExportKind_t kind) {
  gpuTKExport_t exprt;

  if (file == nullptr) {
    gpuTKLog(ERROR, "Go NULL for file value.");
    gpuTKExit();
  }

  gpuTKExport_setFile(exprt, NULL);
  gpuTKExport_setKind(exprt, kind);

  if (kind == gpuTKExportKind_raw) {
    gpuTKExportRaw_t raw = gpuTKExportRaw_new();
    gpuTKExportRaw_setFile(raw, file);
    gpuTKExport_setRaw(exprt, raw);
  } else if (kind == gpuTKExportKind_text) {
    gpuTKExportText_t txt = gpuTKExportText_new();
    gpuTKExportText_setFile(txt, file);
    gpuTKExport_setText(exprt, txt);
  } else if (kind == gpuTKExportKind_tsv || kind == gpuTKExportKind_csv) {
    gpuTKExportCSV_t csv = gpuTKExportCSV_new();
    if (kind == gpuTKExportKind_csv) {
      gpuTKExportCSV_setSeperator(csv, ',');
    } else {
      gpuTKExportCSV_setSeperator(csv, '\t');
    }
    gpuTKExportCSV_setFile(csv, file);
    gpuTKExport_setCSV(exprt, csv);
  } else if (kind == gpuTKExportKind_ppm) {
    gpuTKExport_setFile(exprt, gpuTKString_duplicate(file));
  } else {
    gpuTKLog(ERROR, "Invalid export type.");
    gpuTKExit();
  }

  return exprt;
}

static inline gpuTKExport_t gpuTKExport_open(const char *file,
                                       const char *type0) {
  gpuTKExport_t exprt;
  gpuTKExportKind_t kind;
  char *type;

  type = gpuTKString_toLower(type0);

  if (gpuTKString_sameQ(type, "csv")) {
    kind = gpuTKExportKind_csv;
  } else if (gpuTKString_sameQ(type, "tsv")) {
    kind = gpuTKExportKind_tsv;
  } else if (gpuTKString_sameQ(type, "raw") || gpuTKString_sameQ(type, "dat")) {
    kind = gpuTKExportKind_raw;
  } else if (gpuTKString_sameQ(type, "ppm") || gpuTKString_sameQ(type, "pbm")) {
    kind = gpuTKExportKind_ppm;
  } else if (gpuTKString_sameQ(type, "txt") || gpuTKString_sameQ(type, "text")) {
    kind = gpuTKExportKind_text;
  } else {
    gpuTKLog(ERROR, "Invalid export type ", type0);
    gpuTKExit();
  }

  exprt = gpuTKExport_open(file, kind);

  gpuTKDelete(type);

  return exprt;
}

static inline void gpuTKExport_close(gpuTKExport_t exprt) {
  gpuTKExportKind_t kind;

  kind = gpuTKExport_getKind(exprt);

  if (gpuTKExport_getFile(exprt)) {
    gpuTKDelete(gpuTKExport_getFile(exprt));
  }

  if (kind == gpuTKExportKind_tsv || kind == gpuTKExportKind_csv) {
    gpuTKExportCSV_t csv = gpuTKExport_getCSV(exprt);
    gpuTKExportCSV_delete(csv);
    gpuTKExport_setCSV(exprt, NULL);
  } else if (kind == gpuTKExportKind_raw) {
    gpuTKExportRaw_t raw = gpuTKExport_getRaw(exprt);
    gpuTKExportRaw_delete(raw);
    gpuTKExport_setRaw(exprt, NULL);
  } else if (kind == gpuTKExportKind_text) {
    gpuTKExportText_t text = gpuTKExport_getText(exprt);
    gpuTKExportText_delete(text);
    gpuTKExport_setText(exprt, NULL);
  } else if (kind == gpuTKExportKind_ppm) {
  } else {
    gpuTKLog(ERROR, "Invalid export type.");
    gpuTKExit();
  }
  return;
}

static inline void gpuTKExport_writeAsImage(gpuTKExport_t exprt, gpuTKImage_t img) {
  gpuTKAssert(gpuTKExport_getKind(exprt) == gpuTKExportKind_ppm);

  gpuTKPPM_export(gpuTKExport_getFile(exprt), img);

  return;
}

static inline void gpuTKExport_write(gpuTKExport_t exprt, void *data, int rows,
                                  int columns, char sep, gpuTKType_t type) {
  gpuTKExportKind_t kind;

  kind = gpuTKExport_getKind(exprt);
  if (kind == gpuTKExportKind_tsv || kind == gpuTKExportKind_csv) {
    gpuTKExportCSV_t csv = gpuTKExport_getCSV(exprt);
    gpuTKExportCSV_write(csv, data, rows, columns, sep, type);
  } else if (kind == gpuTKExportKind_raw) {
    gpuTKExportRaw_t raw = gpuTKExport_getRaw(exprt);
    gpuTKExportRaw_write(raw, data, rows, columns, type);
  } else if (kind == gpuTKExportKind_text) {
    gpuTKExportText_t text = gpuTKExport_getText(exprt);
    if (columns == 0) {
      columns = 1;
    }
    if (rows == 0) {
      rows = 1;
    }
    gpuTKExportText_write(text, (const char *)data, rows * columns);
  } else {
    gpuTKLog(ERROR, "Invalid export type.");
    gpuTKExit();
  }
  return;
}

static inline void gpuTKExport_write(gpuTKExport_t exprt, void *data, int rows,
                                  int columns, gpuTKType_t type) {
  gpuTKExport_write(exprt, data, rows, columns, ',', type);
}

static gpuTKExportKind_t _parseExportExtension(const char *file) {
  char *extension;
  gpuTKExportKind_t kind;

  extension = gpuTKFile_extension(file);

  if (gpuTKString_sameQ(extension, "csv")) {
    kind = gpuTKExportKind_csv;
  } else if (gpuTKString_sameQ(extension, "tsv")) {
    kind = gpuTKExportKind_tsv;
  } else if (gpuTKString_sameQ(extension, "raw") ||
             gpuTKString_sameQ(extension, "dat")) {
    kind = gpuTKExportKind_raw;
  } else if (gpuTKString_sameQ(extension, "text") ||
             gpuTKString_sameQ(extension, "txt")) {
    kind = gpuTKExportKind_text;
  } else if (gpuTKString_sameQ(extension, "ppm") ||
             gpuTKString_sameQ(extension, "pbm")) {
    kind = gpuTKExportKind_ppm;
  } else {
    kind = gpuTKExportKind_unknown;
    gpuTKLog(ERROR, "File ", file, " does not have a compatible extension.");
  }

  gpuTKDelete(extension);

  return kind;
}

static void gpuTKExport(const char *file, void *data, int rows, int columns,
                     gpuTKType_t type) {
  gpuTKExportKind_t kind;
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  kind  = _parseExportExtension(file);
  exprt = gpuTKExport_open(file, kind);

  gpuTKExport_write(exprt, data, rows, columns, type);
  gpuTKExport_close(exprt);
}

void gpuTKExport(const char *file, unsigned char *data, int rows) {
  gpuTKExport(file, data, rows, 1);
  return;
}

void gpuTKExport(const char *file, int *data, int rows) {
  gpuTKExport(file, data, rows, 1);
  return;
}

void gpuTKExport(const char *file, gpuTKReal_t *data, int rows) {
  gpuTKExport(file, data, rows, 1);
  return;
}

void gpuTKExport(const char *file, unsigned char *data, int rows,
              int columns) {
  gpuTKExportKind_t kind;
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  kind  = _parseExportExtension(file);
  exprt = gpuTKExport_open(file, kind);

  gpuTKExport_write(exprt, data, rows, columns, gpuTKType_ubit8);
  gpuTKExport_close(exprt);
}

void gpuTKExport(const char *file, int *data, int rows, int columns) {
  gpuTKExportKind_t kind;
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  kind  = _parseExportExtension(file);
  exprt = gpuTKExport_open(file, kind);

  gpuTKExport_write(exprt, data, rows, columns, gpuTKType_integer);
  gpuTKExport_close(exprt);
}

void gpuTKExport(const char *file, gpuTKReal_t *data, int rows, int columns) {
  gpuTKExportKind_t kind;
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  kind  = _parseExportExtension(file);
  exprt = gpuTKExport_open(file, kind);

  gpuTKExport_write(exprt, data, rows, columns, gpuTKType_real);
  gpuTKExport_close(exprt);
}

void gpuTKExport(const char *file, gpuTKExportKind_t kind, void *data, int rows,
              int columns, gpuTKType_t type) {
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  exprt = gpuTKExport_open(file, kind);

  gpuTKExport_write(exprt, data, rows, columns, type);
  gpuTKExport_close(exprt);
}

void gpuTKExport(const char *file, gpuTKImage_t img) {
  gpuTKExportKind_t kind;
  gpuTKExport_t exprt;

  if (file == nullptr) {
    return;
  }

  kind  = _parseExportExtension(file);
  exprt = gpuTKExport_open(file, kind);

  gpuTKAssert(kind == gpuTKExportKind_ppm);

  gpuTKExport_writeAsImage(exprt, img);
  gpuTKExport_close(exprt);
}

void gpuTKExport_text(const char *file, void *data, int length) {
  gpuTKExport(file, gpuTKExportKind_text, data, 1, length, gpuTKType_ascii);
}
