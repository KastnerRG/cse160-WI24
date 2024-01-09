
#include "gputk.h"

#define MB (1 << 20)
#ifndef GPUTK_DEFAULT_HEAP_SIZE
#define GPUTK_DEFAULT_HEAP_SIZE (1024 * MB)
#endif /* GPUTK_DEFAULT_HEAP_SIZE */

static bool _initializedQ = gpuTKFalse;

#ifndef GPUTK_USE_WINDOWS
//__attribute__((__constructor__))
#endif /* GPUTK_USE_WINDOWS */
void gpuTK_init(int *
#ifdef GPUTK_USE_MPI
                 argc
#endif /* GPUTK_USE_MPI */
             ,
             char ***
#ifdef GPUTK_USE_MPI
                 argv
#endif /* GPUTK_USE_MPI */
             ) {
  if (_initializedQ == gpuTKTrue) {
    return;
  }
#ifdef GPUTK_USE_MPI
  gpuTKMPI_Init(argc, argv);
#endif /* GPUTK_USE_MPI */

  _envSessionId();
#ifdef GPUTK_USE_CUDA
  CUresult err = cuInit(0);

/* Select a random GPU */

#ifdef GPUTK_USE_MPI
  if (rankCount() > 1) {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    srand(time(NULL));
    cudaSetDevice(gpuTKMPI_getRank() % deviceCount);
  } else {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);

    srand(time(NULL));
    cudaSetDevice(rand() % deviceCount);
  }
#else
  {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);

    srand(time(NULL));
    cudaSetDevice(rand() % deviceCount);
  }
#endif /* GPUTK_USE_MPI */

  cudaDeviceSetLimit(cudaLimitPrintfFifoSize, 1 * MB);
  cudaDeviceSetLimit(cudaLimitMallocHeapSize, GPUTK_DEFAULT_HEAP_SIZE);

  cudaDeviceSynchronize();

#endif /* GPUTK_USE_CUDA */

#ifdef GPUTK_USE_WINDOWS
  QueryPerformanceFrequency((LARGE_INTEGER *)&_hrtime_frequency);
#endif /* _MSC_VER */

  _hrtime();

  _timer        = gpuTKTimer_new();
  _logger       = gpuTKLogger_new();
  _initializedQ = gpuTKTrue;

  gpuTKFile_init();

  solutionJSON = nullptr;

#ifdef GPUTK_USE_MPI
  atexit(gpuTKMPI_Exit);
#else  /* GPUTK_USE_MPI */
  atexit(gpuTK_atExit);
#endif /* GPUTK_USE_MPI */
}
