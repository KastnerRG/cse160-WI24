#include <gputk.h>

//@@ The purpose of this code is to become familiar with the submission
//@@ process. Do not worry if you do not understand all the details of
//@@ the code.

int main(int argc, char **argv) {
  int deviceCount;

  gpuTKArg_read(argc, argv);

  cudaGetDeviceCount(&deviceCount);

  gpuTKTime_start(GPU, "Getting GPU Data."); //@@ start a timer

  for (int dev = 0; dev < deviceCount; dev++) {
    cudaDeviceProp deviceProp;

    cudaGetDeviceProperties(&deviceProp, dev);

    if (dev == 0) {
      if (deviceProp.major == 9999 && deviceProp.minor == 9999) {
        gpuTKLog(TRACE, "No CUDA GPU has been detected");
        return -1;
      } else if (deviceCount == 1) {
        //@@ gpuTKLog is a provided logging API (similar to Log4J).
        //@@ The logging function gpuTKLog takes a level which is either
        //@@ OFF, FATAL, ERROR, WARN, INFO, DEBUG, or TRACE and a
        //@@ message to be printed.
        gpuTKLog(TRACE, "There is 1 device supporting CUDA");
      } else {
        gpuTKLog(TRACE, "There are ", deviceCount,
              " devices supporting CUDA");
      }
    }

    gpuTKLog(TRACE, "Device ", dev, " name: ", deviceProp.name);
    gpuTKLog(TRACE, " Computational Capabilities: ", deviceProp.major, ".",
          deviceProp.minor);
    gpuTKLog(TRACE, " Maximum global memory size: ",
          deviceProp.totalGlobalMem);
    gpuTKLog(TRACE, " Maximum constant memory size: ",
          deviceProp.totalConstMem);
    gpuTKLog(TRACE, " Maximum shared memory size per block: ",
          deviceProp.sharedMemPerBlock);
    gpuTKLog(TRACE, " Maximum block dimensions: ",
          deviceProp.maxThreadsDim[0], " x ", deviceProp.maxThreadsDim[1],
          " x ", deviceProp.maxThreadsDim[2]);
    gpuTKLog(TRACE, " Maximum grid dimensions: ", deviceProp.maxGridSize[0],
          " x ", deviceProp.maxGridSize[1], " x ",
          deviceProp.maxGridSize[2]);
    gpuTKLog(TRACE, " Warp size: ", deviceProp.warpSize);
  }

  gpuTKTime_stop(GPU, "Getting GPU Data."); //@@ stop the timer

  return 0;
}
