

#ifndef __GPUTK_FILE_H__
#define __GPUTK_FILE_H__

struct st_gpuTKFile_t {
  int index;
  char *file;
  char *mode;
  char *data;
  FILE *handle;
  size_t len;
  size_t offset;
};

#define gpuTKFile_getIndex(file) ((file)->index)
#define gpuTKFile_getFileName(file) ((file)->file)
#define gpuTKFile_getMode(file) ((file)->mode)
#define gpuTKFile_getData(file) ((file)->data)
#define gpuTKFile_getLength(file) ((file)->len)
#define gpuTKFile_getDataOffset(file) ((file)->offset)
#define gpuTKFile_getFileHandle(file) ((file)->handle)

#define gpuTKFile_setIndex(file, val) (gpuTKFile_getIndex(file) = val)
#define gpuTKFile_setFileName(file, val) (gpuTKFile_getFileName(file) = val)
#define gpuTKFile_setMode(file, val) (gpuTKFile_getMode(file) = val)
#define gpuTKFile_setData(file, val) (gpuTKFile_getData(file) = val)
#define gpuTKFile_setLength(file, val) (gpuTKFile_getLength(file) = val)
#define gpuTKFile_setDataOffset(file, val) (gpuTKFile_getDataOffset(file) = val)
#define gpuTKFile_setFileHandle(file, val) (gpuTKFile_getFileHandle(file) = val)

gpuTKFile_t gpuTKFile_new(void);
void gpuTKFile_delete(gpuTKFile_t file);
void gpuTKFile_close(gpuTKFile_t file);
void gpuTKFile_init(void);
void gpuTKFile_atExit(void);
int gpuTKFile_count(void);
gpuTKFile_t gpuTKFile_open(const char *fileName, const char *mode);
gpuTKFile_t gpuTKFile_open(const char *fileName);
char *gpuTKFile_read(gpuTKFile_t file, size_t size, size_t count);
char *gpuTKFile_read(gpuTKFile_t file, size_t len);
void gpuTKFile_rewind(gpuTKFile_t file);
size_t gpuTKFile_size(gpuTKFile_t file);
char *gpuTKFile_read(gpuTKFile_t file);
char *gpuTKFile_readLine(gpuTKFile_t file);
void gpuTKFile_write(gpuTKFile_t file, const void *buffer, size_t size,
                  size_t count);
void gpuTKFile_write(gpuTKFile_t file, const void *buffer, size_t len);
void gpuTKFile_write(gpuTKFile_t file, const char *buffer);
void gpuTKFile_writeLine(gpuTKFile_t file, const char *buffer0);
void gpuTKFile_write(gpuTKFile_t file, string buffer);
void gpuTKFile_writeLine(gpuTKFile_t file, string buffer0);
gpuTKBool gpuTKFile_existsQ(const char *path);
char *gpuTKFile_extension(const char *file);

#endif /* __GPUTK_FILE_H__ */
