#include "fc_cust.h"

void FC_Custom::init() {
  weight.resize(dim_in, dim_out);
  bias.resize(dim_out);
  grad_weight.resize(dim_in, dim_out);
  grad_bias.resize(dim_out);
  set_normal_random(weight.data(), weight.size(), 0, 0.01);
  set_normal_random(bias.data(), bias.size(), 0, 0.01);
}

void FC_Custom::forward(const Matrix& bottom) {

  const int NUM = bottom.cols();
  top.resize(dim_out, NUM);

  float *x = (float*)bottom.data();
  float *y = (float*)top.data();
  float *w = (float*)weight.data();
  float *b = (float*)bias.data();  

  const int DIM_in = dim_in;
  const int DIM_out = dim_out;

  
  float* x_d;
  float* y_d;
  float* w_d;
  float* b_d;

  std::cout<<"FC-GPU=="<<std::endl;

  // Launch marker kernel to aid with student function timing
  gpuUtils.insert_pre_barrier_kernel();
  
  // Start layer timer
  auto start_time_layer = std::chrono::high_resolution_clock::now();
  // Data transfer CPU to GPU
  gpuInterface.fc_forward_gpu_prolog(y, x, w, b, &y_d, &x_d, &w_d, &b_d, NUM, DIM_in, DIM_out);

  // Start kernel timer
  auto start_time_kernel = std::chrono::high_resolution_clock::now();
  // Hand off to GPU for computation
  gpuInterface.fc_forward_gpu(y_d, x_d, w_d, b_d, NUM, DIM_in, DIM_out);
  cudaDeviceSynchronize();
  // Stop kernel timer
  auto end_time_kernel = std::chrono::high_resolution_clock::now();
  
  // Data transfer GPU to CPU
  gpuInterface.fc_forward_gpu_epilog(y, y_d, x_d, w_d, b_d, NUM, DIM_in, DIM_out);

  // Stop layer timer
  auto end_time_layer = std::chrono::high_resolution_clock::now();

  // Launch barrier kernel to aid with timing with nsight-compute
  gpuUtils.insert_post_barrier_kernel();

  std::chrono::duration<float, std::milli> duration_layer = (end_time_layer-start_time_layer);
  std::cout<<"FC Layer Time: " << duration_layer.count() << " ms"<<std::endl;
  
  std::chrono::duration<float, std::milli> duration_kernel = (end_time_kernel-start_time_kernel);
  std::cout<<"FC Op Time: " << duration_kernel.count() << " ms"<<std::endl;

}

void FC_Custom::backward(const Matrix& bottom, const Matrix& grad_top) {
  
}

void FC_Custom::update(Optimizer& opt) {
  
}

std::vector<float> FC_Custom::get_parameters() const {
  std::vector<float> res(weight.size() + bias.size());
  // Copy the data of weights and bias to a long vector
  std::copy(weight.data(), weight.data() + weight.size(), res.begin());
  std::copy(bias.data(), bias.data() + bias.size(),
            res.begin() + weight.size());
  return res;
}

void FC_Custom::set_parameters(const std::vector<float>& param) {
  if (static_cast<int>(param.size()) != weight.size() + bias.size())
      throw std::invalid_argument("Parameter size does not match");
  std::copy(param.begin(), param.begin() + weight.size(), weight.data());
  std::copy(param.begin() + weight.size(), param.end(), bias.data());
}

std::vector<float> FC_Custom::get_derivatives() const {
  std::vector<float> res(grad_weight.size() + grad_bias.size());
  // Copy the data of weights and bias to a long vector
  std::copy(grad_weight.data(), grad_weight.data() + grad_weight.size(),
            res.begin());
  std::copy(grad_bias.data(), grad_bias.data() + grad_bias.size(),
            res.begin() + grad_weight.size());
  return res;
}
