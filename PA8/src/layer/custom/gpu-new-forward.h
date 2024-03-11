#ifndef SRC_LAYER_GPU_NEW_FORWARD_H
#define SRC_LAYER_GPU_NEW_FORWARD_H

class GPUInterface
{
    public:
    void get_device_properties();
    // Convolution
    void conv_forward_gpu_prolog(const float *host_y, const float *host_x, const float *host_k, float **device_y_ptr, float **device_x_ptr, float **device_k_ptr, const int B, const int M, const int C, const int H, const int W, const int K);
    void conv_forward_gpu(float *device_y, const float *device_x, const float *device_k, const int B, const int M, const int C, const int H, const int W, const int K);
    void conv_forward_gpu_epilog(float *host_y, float *device_y, float *device_x, float *device_k, const int B, const int M, const int C, const int H, const int W, const int K);

    // FC
    void fc_forward_gpu_prolog(const float *host_y, const float *host_x, const float *host_w, const float *host_b, float **device_y_ptr, float **device_x_ptr, float **device_w_ptr, float **device_b_ptr, const int NUM, const int DIM_in, const int DIM_out);
    void fc_forward_gpu(float *device_y, const float *device_x, const float *device_w, float *device_b, const int NUM, const int DIM_in, const int DIM_out);
    void fc_forward_gpu_epilog(float *host_y, float *device_y, float *device_x, float *device_w, float *device_b, const int NUM, const int DIM_in, const int DIM_out);

};

#endif
