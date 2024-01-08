#ifndef SRC_MNIST_H_
#define SRC_MNIST_H_

#include <fstream>
#include <iostream>
#include <string>
#include "./utils.h"

class MNIST {
 private:
  std::string data_dir;

 public:
  Matrix train_data;
  Matrix train_labels;
  Matrix test_data;
  Matrix test_labels;

  void read_mnist_data(std::string filename, Matrix& data, int batch_size=-1);
  void read_mnist_label(std::string filename, Matrix& labels, int batch_size=-1);

  explicit MNIST(std::string data_dir) : data_dir(data_dir) {}
  void read();
  void read_test_data(int batch_size); 
};

#endif  // SRC_MNIST_H_
