

#ifndef __gpuTKPPM_H__
#define __gpuTKPPM_H__

START_EXTERN_C
gpuTKImage_t gpuTKPPM_import(const char *filename);
void gpuTKPPM_export(const char *filename, gpuTKImage_t img);
END_EXTERN_C

#endif /* __gpuTKPPM_H__ */
