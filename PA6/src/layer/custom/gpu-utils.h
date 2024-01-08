#ifndef SRC_LAYER_GPU_UTILS_H
#define SRC_LAYER_GPU_UTILS_H

// Use this class to define GPU functions that students don't need access to. 
class GPU_Utils
{
    public:
    /* For creating a dummy kernel call so that we can distinguish between kernels launched for different layers
     * in the Nsight Compute CLI for measuring per layer Op Times
     */
    void insert_post_barrier_kernel();
    // For inserting a marker visible in Nsys so that we can time total student function time
    void insert_pre_barrier_kernel();
};

#endif 