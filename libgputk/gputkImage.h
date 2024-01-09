

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "gputkTypes.h"

struct st_gpuTKImage_t {
  int width;
  int height;
  int channels;
  int pitch;
  float *data;
};

#define gpuTKImage_channels 3

#define gpuTKImage_getWidth(img) ((img)->width)
#define gpuTKImage_getHeight(img) ((img)->height)
#define gpuTKImage_getChannels(img) ((img)->channels)
#define gpuTKImage_getPitch(img) ((img)->pitch)
#define gpuTKImage_getData(img) ((img)->data)

#define gpuTKImage_setWidth(img, val) (gpuTKImage_getWidth(img) = val)
#define gpuTKImage_setHeight(img, val) (gpuTKImage_getHeight(img) = val)
#define gpuTKImage_setChannels(img, val) (gpuTKImage_getChannels(img) = val)
#define gpuTKImage_setPitch(img, val) (gpuTKImage_getPitch(img) = val)
#define gpuTKImage_setData(img, val) (gpuTKImage_getData(img) = val)

typedef void (*gpuTKImage_onSameFunction_t)(string str);

gpuTKImage_t gpuTKImage_new(int width, int height, int channels, float *data);
gpuTKImage_t gpuTKImage_new(int width, int height, int channels);
gpuTKImage_t gpuTKImage_new(int width, int height);
void gpuTKImage_delete(gpuTKImage_t img);
gpuTKBool gpuTKImage_sameQ(gpuTKImage_t a, gpuTKImage_t b,
                     gpuTKImage_onSameFunction_t onUnSame);
gpuTKBool gpuTKImage_sameQ(gpuTKImage_t a, gpuTKImage_t b);

#endif /* __IMAGE_H__ */
