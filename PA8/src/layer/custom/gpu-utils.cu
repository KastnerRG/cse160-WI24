#include "gpu-utils.h"

__global__ void do_not_remove_this_kernel() {
    int tx = threadIdx.x;
    tx = tx + 1;
}

__global__ void prefn_marker_kernel() {
    int tx = threadIdx.x;
    tx = tx + 1;
}

__host__ void GPU_Utils::insert_post_barrier_kernel() {
    
    dim3 GridDim(1,1,1);
    dim3 BlockDim(1,1,1);
    do_not_remove_this_kernel<<<GridDim, BlockDim>>>();
    cudaDeviceSynchronize();
}

__host__ void GPU_Utils::insert_pre_barrier_kernel() {

    int* devicePtr;
    int x = 1;

    cudaMalloc((void**) &devicePtr, sizeof(int));
    cudaMemcpy(devicePtr, &x, sizeof(int), cudaMemcpyHostToDevice);

    dim3 GridDim(1,1,1);
    dim3 BlockDim(1,1,1);
    prefn_marker_kernel<<<GridDim, BlockDim>>>();
    cudaFree(devicePtr);
    cudaDeviceSynchronize();
}