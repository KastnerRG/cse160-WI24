#ifndef __GPUTK_DIRECTORY__
#define __GPUTK_DIRECTORY__

extern const char gpuTKDirectorySeperator;
EXTERN_C char *gpuTKDirectory_name(const char *pth);
EXTERN_C const char *gpuTKDirectory_create(const char *dir);
EXTERN_C char *gpuTKDirectory_current();

#endif /* __GPUTK_DIRECTORY__ */
