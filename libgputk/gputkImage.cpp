

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

gpuTKImage_t gpuTKImage_new(int width, int height, int channels, float *data) {
  gpuTKImage_t img;

  img = gpuTKNew(struct st_gpuTKImage_t);

  gpuTKImage_setWidth(img, width);
  gpuTKImage_setHeight(img, height);
  gpuTKImage_setChannels(img, channels);
  gpuTKImage_setPitch(img, width * channels);

  gpuTKImage_setData(img, data);
  return img;
}

gpuTKImage_t gpuTKImage_new(int width, int height, int channels) {
  float *data = gpuTKNewArray(float, width *height *channels);
  return gpuTKImage_new(width, height, channels, data);
}

gpuTKImage_t gpuTKImage_new(int width, int height) {
  return gpuTKImage_new(width, height, gpuTKImage_channels);
}

void gpuTKImage_delete(gpuTKImage_t img) {
  if (img != nullptr) {
    if (gpuTKImage_getData(img) != nullptr) {
      gpuTKDelete(gpuTKImage_getData(img));
    }
    gpuTKDelete(img);
  }
}

static inline void gpuTKImage_setPixel(gpuTKImage_t img, int x, int y, int c,
                                    float val) {
  float *data  = gpuTKImage_getData(img);
  int channels = gpuTKImage_getChannels(img);
  int pitch    = gpuTKImage_getPitch(img);

  data[y * pitch + x * channels + c] = val;

  return;
}

static inline float gpuTKImage_getPixel(gpuTKImage_t img, int x, int y, int c) {
  float *data  = gpuTKImage_getData(img);
  int channels = gpuTKImage_getChannels(img);
  int pitch    = gpuTKImage_getPitch(img);

  return data[y * pitch + x * channels + c];
}

gpuTKBool gpuTKImage_sameQ(gpuTKImage_t a, gpuTKImage_t b,
                     gpuTKImage_onSameFunction_t onUnSame) {
  if (a == nullptr || b == nullptr) {
    gpuTKLog(ERROR, "Comparing null images.");
    return gpuTKFalse;
  } else if (a == b) {
    return gpuTKTrue;
  } else if (gpuTKImage_getWidth(a) != gpuTKImage_getWidth(b)) {
    gpuTKLog(ERROR, "Image widths do not match.");
    return gpuTKFalse;
  } else if (gpuTKImage_getHeight(a) != gpuTKImage_getHeight(b)) {
    gpuTKLog(ERROR, "Image heights do not match.");
    return gpuTKFalse;
  } else if (gpuTKImage_getChannels(a) != gpuTKImage_getChannels(b)) {
    gpuTKLog(ERROR, "Image channels do not match.");
    return gpuTKFalse;
  } else {
    float *aData, *bData;
    int width, height, channels;
    int ii, jj, kk;

    aData = gpuTKImage_getData(a);
    bData = gpuTKImage_getData(b);

    gpuTKAssert(aData != nullptr);
    gpuTKAssert(bData != nullptr);

    width    = gpuTKImage_getWidth(a);
    height   = gpuTKImage_getHeight(a);
    channels = gpuTKImage_getChannels(a);

    for (ii = 0; ii < height; ii++) {
      for (jj = 0; jj < width; jj++) {
        for (kk = 0; kk < channels; kk++) {
          float x, y;
          if (channels <= 3) {
            x = _clamp(*aData++, 0, 1);
            y = _clamp(*bData++, 0, 1);
          } else {
            x = *aData++;
            y = *bData++;
          }
          if (gpuTKUnequalQ(x, y)) {
            if (onUnSame != nullptr) {
              string str = gpuTKString(
                  "Image pixels do not match at position ( row = ",
                  gpuTKString(ii, ", col = ", jj, ", channel = ", kk,
                           ") expecting a value of "),
                  gpuTKString(y, " but got a computed value of ", x));
              onUnSame(str);
            }
            return gpuTKFalse;
          }
        }
      }
    }
    return gpuTKTrue;
  }
}

static void gpuTKImage_onUnsameFunction(string str) {
  gpuTKLog(ERROR, str);
}

gpuTKBool gpuTKImage_sameQ(gpuTKImage_t a, gpuTKImage_t b) {
  return gpuTKImage_sameQ(a, b, gpuTKImage_onUnsameFunction);
}
