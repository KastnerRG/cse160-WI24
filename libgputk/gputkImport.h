
#ifndef __GPUTK_IMPORT_H__
#define __GPUTK_IMPORT_H__

#include "gputkImage.h"

typedef enum en_gpuTKImportKind_t {
  gpuTKImportKind_unknown = -1,
  gpuTKImportKind_raw     = 0x1000,
  gpuTKImportKind_csv,
  gpuTKImportKind_tsv,
  gpuTKImportKind_ppm,
  gpuTKImportKind_text
} gpuTKImportKind_t;

#define gpuTKType_real gpuTKType_float

typedef struct st_gpuTKImportCSV_t {
  int rows;
  int columns;
  void *data;
  gpuTKFile_t file;
  char seperator;
} * gpuTKImportCSV_t;

#define gpuTKImportCSV_getRowCount(csv) ((csv)->rows)
#define gpuTKImportCSV_getColumnCount(csv) ((csv)->columns)
#define gpuTKImportCSV_getData(csv) ((csv)->data)
#define gpuTKImportCSV_getFile(csv) ((csv)->file)
#define gpuTKImportCSV_getSeperator(csv) ((csv)->seperator)

#define gpuTKImportCSV_setRowCount(csv, val)                                 \
  (gpuTKImportCSV_getRowCount(csv) = val)
#define gpuTKImportCSV_setColumnCount(csv, val)                              \
  (gpuTKImportCSV_getColumnCount(csv) = val)
#define gpuTKImportCSV_setData(csv, val) (gpuTKImportCSV_getData(csv) = val)
#define gpuTKImportCSV_setSeperator(csv, val)                                \
  (gpuTKImportCSV_getSeperator(csv) = val)

typedef struct st_gpuTKImportRaw_t {
  int rows;
  int columns;
  void *data;
  gpuTKFile_t file;
} * gpuTKImportRaw_t;

#define gpuTKImportRaw_getRowCount(raw) ((raw)->rows)
#define gpuTKImportRaw_getColumnCount(raw) ((raw)->columns)
#define gpuTKImportRaw_getData(raw) ((raw)->data)
#define gpuTKImportRaw_getFile(raw) ((raw)->file)

#define gpuTKImportRaw_setRowCount(raw, val)                                 \
  (gpuTKImportRaw_getRowCount(raw) = val)
#define gpuTKImportRaw_setColumnCount(raw, val)                              \
  (gpuTKImportRaw_getColumnCount(raw) = val)
#define gpuTKImportRaw_setData(raw, val) (gpuTKImportRaw_getData(raw) = val)

typedef struct st_gpuTKImportText_t {
  int length;
  char *data;
  gpuTKFile_t file;
} * gpuTKImportText_t;

#define gpuTKImportText_getLength(txt) ((txt)->length)
#define gpuTKImportText_getData(txt) ((txt)->data)
#define gpuTKImportText_getFile(txt) ((txt)->file)

#define gpuTKImportText_setLength(txt, val)                                  \
  (gpuTKImportText_getLength(txt) = val)
#define gpuTKImportText_setData(txt, val) (gpuTKImportText_getData(txt) = val)

typedef struct st_gpuTKImport_t {
  gpuTKImportKind_t kind;
  union {
    gpuTKImportRaw_t raw;
    gpuTKImportCSV_t csv;
    gpuTKImportText_t text;
    gpuTKImage_t img;
  } container;
} gpuTKImport_t;

#define gpuTKImport_getKind(imp) ((imp).kind)
#define gpuTKImport_getContainer(imp) ((imp).container)
#define gpuTKImport_getRaw(imp) (gpuTKImport_getContainer(imp).raw)
#define gpuTKImport_getCSV(imp) (gpuTKImport_getContainer(imp).csv)
#define gpuTKImport_getText(imp) (gpuTKImport_getContainer(imp).text)
#define gpuTKImport_getImage(imp) (gpuTKImport_getContainer(imp).img)

#define gpuTKImport_setKind(imp, val) (gpuTKImport_getKind(imp) = val)
#define gpuTKImport_setRaw(imp, val) (gpuTKImport_getRaw(imp) = val)
#define gpuTKImport_setCSV(imp, val) (gpuTKImport_getCSV(imp) = val)
#define gpuTKImport_setText(imp, val) (gpuTKImport_getText(imp) = val)
#define gpuTKImport_setImage(imp, val) (gpuTKImport_getImage(imp) = val)

EXTERN_C void *gpuTKImport(const char *file, int *rows);
void *gpuTKImport(const char *file, int *rows, int *columns);
void *gpuTKImport(const char *file, int *rows, const char *type);
void *gpuTKImport(const char *file, int *resRows, int *resColumns,
               const char *type);
gpuTKImage_t gpuTKImport(const char *file);
int gpuTKImport_flag(const char *file);

#endif /* __GPUTK_IMPORT_H__ */
