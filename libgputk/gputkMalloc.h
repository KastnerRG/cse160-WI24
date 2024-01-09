

#ifndef __GPUTK_MALLOC_H__
#define __GPUTK_MALLOC_H__

#ifdef GPUTK_USE_CUSTOM_MALLOC
#ifdef __linux__
#define THROW __THROW
#else
#define THROW
#endif

static inline void *_malloc(size_t size) THROW {
  if (size == 0) {
    return NULL;
  } else {
    int err;
    void *res = memmgr_alloc((ulong)size, &err);
    if (err) {
      fprintf(stderr, "<<MEMORY>>:: Memory allocation failed\n");
      exit(1);
    } else {
      size_t ii        = 0;
      unsigned char *p = (unsigned char *)res;
      while (ii++ < size) {
        *p++ = 0;
      }
      return res;
    }
  }
}

static inline void _free(void *ptr) THROW {
  if (ptr != nullptr) {
    memmgr_free(ptr);
  }
}

static inline void *_calloc(size_t nmemb, size_t size) THROW {
  return _malloc(nmemb * size);
}

static inline void *_realloc(void *ptr, size_t size) THROW {
  if (size == 0) {
    free(ptr);
    return NULL;
  } else if (ptr == nullptr) {
    return malloc(size);
  } else {
    void *buf;
    unsigned char *dst;
    unsigned char *src;
    size_t alloc_size, to_copy, i = 0;

    // Allocate new buffer
    buf = malloc(size);

    if (buf != 0) {
      // Find original allocation size
      alloc_size = (size_t)memmgr_get_block_size(ptr);
      to_copy    = alloc_size;
      if (to_copy > size) {
        to_copy = size;
      }

      // Copy data to new buffer
      dst = (unsigned char *)buf;
      src = (unsigned char *)ptr;
      while (i++ < to_copy) {
        *dst++ = *src++;
      }

      // Free the old buffer
      free(ptr);
    }

    return buf;
  }
}

#define gpuTKNew(type) ((type *)_malloc(sizeof(type)))
#define gpuTKNewArray(type, len) ((type *)_malloc((len) * sizeof(type)))
#define gpuTKMalloc(sz) _malloc(sz)
#define gpuTKDelete(var)                                                     \
  _free(var);                                                             \
  var = nullptr
#define gpuTKFree(var)                                                       \
  _free(var);                                                             \
  var = nullptr
#define gpuTKRealloc(var, newSize) _realloc(var, newSize)
#define gpuTKReallocArray(t, m, n) ((t *)_realloc(m, n * sizeof(t)))

#define free _free
#define malloc _malloc
#define calloc _calloc
#define realloc _realloc

#else /* GPUTK_USE_CUSTOM_MALLOC */

static inline void *xMalloc(size_t sz) {
  void *mem = nullptr;
  if (sz != 0) {
    mem = malloc(sz);
  }
  return mem;
}

static inline void xFree(void *mem) {
  if (mem != nullptr) {
    free(mem);
  }
  return;
}

static inline void *xRealloc(void *mem, size_t sz) {
  if (mem == nullptr) {
    return NULL;
  } else if (sz == 0) {
    xFree(mem);
    return NULL;
  } else {
    void *res = realloc(mem, sz);
    gpuTKAssert(res != nullptr);
    return res;
  }
}

#define gpuTKNew(type) ((type *)gpuTKMalloc(sizeof(type)))
#define gpuTKNewArray(type, len) ((type *)gpuTKMalloc((len) * sizeof(type)))
#define gpuTKMalloc(sz) xMalloc(sz)
#define gpuTKDelete(var) gpuTKFree(var)
#define gpuTKFree(var)                                                       \
  xFree(var);                                                             \
  var = nullptr
#define gpuTKRealloc(var, newSize) xRealloc(var, newSize)
#define gpuTKReallocArray(t, m, n) ((t *)xRealloc(m, n * sizeof(t)))

#endif /* GPUTK_USE_CUSTOM_MALLOC */

#endif /* __GPUTK_MALLOC_H__ */
