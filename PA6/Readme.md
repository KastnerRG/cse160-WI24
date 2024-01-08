
# Programming Assignment 6: CNN Forward Layer CPU Implementation

## Objective

This is the first part of a three part project implementing and optimizing the forward pass of a convolution layer using CUDA. Convolutional layers are the primary building blocks of convolutional neural networks (CNNs), which are used for tasks like image classification, object detection, natural language processing and recommendation systems. 

You will be working with a modified version of the LeNet5 architceture shown bellow:

![LenetImage](https://lh5.googleusercontent.com/84RlneM7JSDYDirUr_ceplL4G3-Peyq5dkLJTe2f-3Bj9KuWZjsH2A9Qq5PO5BRLrVfWGPnI3eQu8RkTPgyeUf9ZOWY9JbptVJy9LceAyHRn-O0kbzprx88yb82a5dnCR7EDP7n0)

You can read about the original network:

*Source: http://yann.lecun.com/exdb/publis/pdf/lecun-01a.pdf*

Your optimized CUDA implementation of the convolutional layer will be used to perform inference for layers C1 and C3 (shown in red) in the figure above. This leverages the [mini-dnn-cpp](https://github.com/iamhankai/mini-dnn-cpp) (Mini-DNN) framework for implementing the modified LeNet-5.

## Input Data

The network will be tested on the [Fashion MNIST dataset](https://github.com/zalandoresearch/fashion-mnist) which contains 10,000 single channel images each of dimensions 86x86 but we will only use 1000 of these at a time. The output layer consists of 10 nodes, where each node represents the likelihood of the input belonging to one of the 10 classes (T-shirt, dress, sneaker, boot, etc).

## Instructions

This assignment requires you to complete a CPU implementation of the convolutional layer. Performance of the CPU implementation is not important as this assignment is intended to build your understanding of a naive convolution, the network itself, and how to test your network. The only file you need to update to implement the forward convolution is:
`src/layer/custom/cpu-new-forward.cc`
The psudo-code for this implementation is as follows:
```{.ruby}
for b = 0 .. B                     // for each image in the batch 
    for m = 0 .. M                 // for each output feature maps
        for h = 0 .. H_out         // for each output element
            for w = 0 .. W_out
            {
                y[b][m][h][w] = 0;
                for c = 0 .. C     // sum over all input feature maps
                    for p = 0 .. K // KxK filter
                        for q = 0 .. K
                            y[b][m][h][w] += x[b][c][h + p][w + q] * k[m][c][p][q]
            }
```
This animation can help visualize this process better:
![ConvExample](https://stanford.edu/~shervine/teaching/cs-230/illustrations/convolution-layer-a.png?1c517e00cb8d709baf32fc3d39ebae67)

*Source: https://stanford.edu/~shervine/teaching/cs-230/cheatsheet-convolutional-neural-networks#layer*

## How to Compile

The `src/layer/custom/cpu-new-forward.cc` file contains the code for the programming assignment. There is a Makefile included which compiles it and links it with the libgputk CUDA library automatically. It can be run by typing `make m1` from the PA6 folder. It generates a `m1` output executable.

## How to test

Use the `make run` command to test your program which will run your program on a batch size of 1000 images. This will automatically compile your source (equivilent to executing `make m1` and then running `./m1 1000`).

## Test Output 

You should **NOT** checkout a GPU for this assignment, only the pod iteself which has the correct toolchains. You can accomplish this with:
`launch.sh -g 0 -s -i ucsdets/nvcr-cuda:latest`

After loading a pod, executing `make run` (without having completed `cpu-new-forward.cc`) will result in the following output:

```
nvcc --compile m1.cc -I ../libgputk/ -I./
nvcc --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
nvcc --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
nvcc --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
nvcc -o m1 -lm -lcuda -lrt m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 
./m1 1000
Test batch size: 1000
Loading fashion-mnist data...Done
Loading model...Done
Conv-CPU==
Op Time: 0.0007 ms
Conv-CPU==
Op Time: 0.00044 ms

Test Accuracy: 0.097
```
Without implementing cpu-new-forward.cc the result of the network's classification will be predictably poor. There are two seperate pieces of timing information, one for each of the convolutions (C1 and C3). If you correctly implement the CPU forward layer you will see the following results:
```
nvcc --compile m1.cc -I ../libgputk/ -I./
nvcc --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
nvcc --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
nvcc --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
nvcc -o m1 -lm -lcuda -lrt m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 
./m1 1000
Test batch size: 1000
Loading fashion-mnist data...Done
Loading model...Done
Conv-CPU==
Op Time: 15352.7 ms
Conv-CPU==
Op Time: 41512.8 ms

Test Accuracy: 0.886
```
The accuracy of your implementation should meet the 0.886 of our implementation.

## Submission

Submit the src/layer/custom/cpu-new-forward.cc file on gradescope.

## Credit

This project is originally from UIUC ECE408 and builds off a number of open source projects including the Fashion MNIST dataset, mini-dnn-cpp, and the Eigen project.
