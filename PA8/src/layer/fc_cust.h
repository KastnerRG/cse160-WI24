#ifndef SRC_LAYER_FC_CUST_H_
#define SRC_LAYER_FC_CUST_H_

#include <vector>
#include <chrono>
#include "../layer.h"
#include <cuda_runtime_api.h>
#include "./custom/gpu-new-forward.h"
#include "./custom/gpu-utils.h" 

class FC_Custom : public Layer {
 private:
  const int dim_in;
  const int dim_out;

  Matrix weight;  // weight parameter
  Vector bias;  // bias paramter
  Matrix grad_weight;  // gradient w.r.t weight
  Vector grad_bias;  // gradient w.r.t bias

  GPUInterface gpuInterface;
  GPU_Utils gpuUtils;

  void init();

 public:
  FC_Custom(const int dim_in, const int dim_out) :
                 dim_in(dim_in), dim_out(dim_out)
  { init(); }

  void forward(const Matrix& bottom);
  void backward(const Matrix& bottom, const Matrix& grad_top);
  void update(Optimizer& opt);
  int output_dim() { return dim_out; }
  std::vector<float> get_parameters() const;
  std::vector<float> get_derivatives() const;
  void set_parameters(const std::vector<float>& param);
};

#endif  // SRC_LAYER_FC_CUST_H_