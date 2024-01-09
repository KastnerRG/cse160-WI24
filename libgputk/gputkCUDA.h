
#ifndef __GPUTK_CUDA_H__
#define __GPUTK_CUDA_H__

#ifdef GPUTK_USE_CUDA
#ifdef __PGI
#define __GNUC__ 4
#endif /* __PGI */
#include <cuda.h>
#include <cuda_runtime.h>

typedef struct st_gpuTKCUDAMemory_t {
  void *mem;
  size_t sz;
} gpuTKCUDAMemory_t;

#define _cudaMemoryListSize 1024

extern size_t _cudaMallocSize;
extern gpuTKCUDAMemory_t _cudaMemoryList[];
extern int _cudaMemoryListIdx;

char *gpuTKRandom_list(size_t sz);

static inline cudaError_t gpuTKCUDAMalloc(void **devPtr, size_t sz) {
  int idx = _cudaMemoryListIdx;

  cudaError_t err = cudaMalloc(devPtr, sz);

  if (idx == 0) {
    srand(time(NULL));
    memset(_cudaMemoryList, 0,
           sizeof(gpuTKCUDAMemory_t) * _cudaMemoryListSize);
  }

  if (err == cudaSuccess) {
#if 0
    char * rands = gpuTKRandom_list(sz);
    // can use curand here, but do not want to invoke a kernel
    err = cudaMemcpy(*devPtr, rands, sz, cudaMemcpyHostToDevice);
    gpuTKFree(rands);
#else
    err = cudaMemset(*devPtr, 0, sz);
#endif
  }

  _cudaMallocSize += sz;
  _cudaMemoryList[idx].mem = *devPtr;
  _cudaMemoryList[idx].sz  = sz;
  _cudaMemoryListIdx++;
  return err;
}

static inline cudaError_t gpuTKCUDAFree(void *mem) {
  int idx = _cudaMemoryListIdx;
  if (idx == 0) {
    memset(_cudaMemoryList, 0,
           sizeof(gpuTKCUDAMemory_t) * _cudaMemoryListSize);
  }
  for (int ii = 0; ii < idx; ii++) {
    if (_cudaMemoryList[ii].mem != nullptr &&
        _cudaMemoryList[ii].mem == mem) {
      cudaError_t err = cudaFree(mem);
      _cudaMallocSize -= _cudaMemoryList[ii].sz;
      _cudaMemoryList[ii].mem = nullptr;
      return err;
    }
  }
  return cudaErrorMemoryAllocation;
}

#define cudaMalloc(elem, err) gpuTKCUDAMalloc((void **)elem, err)
#define cudaFree gpuTKCUDAFree

#endif /* GPUTK_USE_CUDA */

#endif /* __GPUTK_CUDA_H__ */
