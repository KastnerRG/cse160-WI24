

#ifndef __GPUTK_EXPORT_H__
#define __GPUTK_EXPORT_H__

#include "gputk.h"
#include "gputkFile.h"
#include "gputkPPM.h"

typedef enum en_gpuTKExportKind_t {
  gpuTKExportKind_unknown = -1,
  gpuTKExportKind_raw     = 0x1000,
  gpuTKExportKind_csv,
  gpuTKExportKind_tsv,
  gpuTKExportKind_ppm,
  gpuTKExportKind_text,
} gpuTKExportKind_t;

typedef struct st_gpuTKExportText_t {
  int length;
  gpuTKFile_t file;
} * gpuTKExportText_t;

#define gpuTKExportText_getLength(txt) ((txt)->length)
#define gpuTKExportText_getFile(txt) ((txt)->file)

#define gpuTKExportText_setLength(txt, val)                                  \
  (gpuTKExportText_getLength(txt) = val)

typedef struct st_gpuTKExportRaw_t {
  int rows;
  int columns;
  gpuTKFile_t file;
} * gpuTKExportRaw_t;

#define gpuTKExportRaw_getColumnCount(raw) ((raw)->columns)
#define gpuTKExportRaw_getRowCount(raw) ((raw)->rows)
#define gpuTKExportRaw_getFile(raw) ((raw)->file)

#define gpuTKExportRaw_setRowCount(raw, val)                                 \
  (gpuTKExportRaw_getRowCount(raw) = val)
#define gpuTKExportRaw_setColumnCount(raw, val)                              \
  (gpuTKExportRaw_getColumnCount(raw) = val)

typedef struct st_gpuTKExportCSV_t {
  int rows;
  int columns;
  gpuTKFile_t file;
  char seperator;
} * gpuTKExportCSV_t;

#define gpuTKExportCSV_getRowCount(csv) ((csv)->rows)
#define gpuTKExportCSV_getColumnCount(csv) ((csv)->columns)
#define gpuTKExportCSV_getFile(csv) ((csv)->file)
#define gpuTKExportCSV_getSeperator(csv) ((csv)->seperator)

#define gpuTKExportCSV_setRowCount(csv, val)                                 \
  (gpuTKExportCSV_getRowCount(csv) = val)
#define gpuTKExportCSV_setColumnCount(csv, val)                              \
  (gpuTKExportCSV_getColumnCount(csv) = val)
#define gpuTKExportCSV_setSeperator(csv, val)                                \
  (gpuTKExportCSV_getSeperator(csv) = val)

typedef struct st_gpuTKExport_t {
  gpuTKExportKind_t kind;
  union {
    gpuTKExportRaw_t raw;
    gpuTKExportCSV_t csv;
    gpuTKImage_t img;
    gpuTKExportText_t text;
  } container;
  char *file;
} gpuTKExport_t;

#define gpuTKExport_getKind(exprt) ((exprt).kind)
#define gpuTKExport_getContainer(exprt) ((exprt).container)
#define gpuTKExport_getRaw(exprt) (gpuTKExport_getContainer(exprt).raw)
#define gpuTKExport_getCSV(exprt) (gpuTKExport_getContainer(exprt).csv)
#define gpuTKExport_getImage(exprt) (gpuTKExport_getContainer(exprt).img)
#define gpuTKExport_getText(exprt) (gpuTKExport_getContainer(exprt).text)
#define gpuTKExport_getFile(exprt) ((exprt).file)

#define gpuTKExport_setKind(exprt, val) (gpuTKExport_getKind(exprt) = val)
#define gpuTKExport_setRaw(exprt, val) (gpuTKExport_getRaw(exprt) = val)
#define gpuTKExport_setCSV(exprt, val) (gpuTKExport_getCSV(exprt) = val)
#define gpuTKExport_setImage(exprt, val) (gpuTKExport_getImage(exprt) = val)
#define gpuTKExport_setText(exprt, val) (gpuTKExport_getText(exprt) = val)
#define gpuTKExport_setFile(exprt, val) (gpuTKExport_getFile(exprt) = val)

void gpuTKExport(const char *file, int *data, int rows, int columns);
void gpuTKExport(const char *file, int *data, int rows);
void gpuTKExport(const char *file, unsigned char *data, int rows,
              int columns);
void gpuTKExport(const char *file, unsigned char *data, int rows);
void gpuTKExport(const char *file, int *data, int rows, int columns);
void gpuTKExport(const char *file, int *data, int rows);
void gpuTKExport(const char *file, gpuTKReal_t *data, int rows, int columns);
void gpuTKExport(const char *file, gpuTKReal_t *data, int rows);
void gpuTKExport(const char *file, gpuTKImage_t img);

void gpuTKExport(const char *file, gpuTKExportKind_t kind, void *data, int rows,
              int columns, gpuTKType_t type);

void gpuTKExport_text(const char *file, void *data, int length);

#endif /* __GPUTK_EXPORT_H__ */
