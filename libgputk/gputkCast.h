

#ifndef __GPUTK_CAST_H__
#define __GPUTK_CAST_H__

template <typename X, typename Y>
static inline void gpuTKCast(X &x, const Y &y, size_t len) {
  size_t ii;

  for (ii = 0; ii < len; ii++) {
    x[ii] = (X)y[ii];
  }

  return;
}

template <typename X, typename Y>
static inline X *gpuTKCast(const Y &y, size_t len) {
  size_t ii;
  X *x = gpuTKNewArray(X, len);

  for (ii = 0; ii < len; ii++) {
    x[ii] = (X)y[ii];
  }

  return x;
}

#endif /* __GPUTK_CAST_H__ */
