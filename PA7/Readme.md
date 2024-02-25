
# Programming Assignment 7: CNN Forward Layer GPU Implementation

## Objective

This is the second part of a three part project implementing and optimizing the forward pass of a convolution layer using CUDA. Convolutional layers are the primary building blocks of convolutional neural networks (CNNs), which are used for tasks like image classification, object detection, natural language processing and recommendation systems. 

You will be working with a modified version of the LeNet5 architceture shown bellow:

![LenetImage](https://lh5.googleusercontent.com/84RlneM7JSDYDirUr_ceplL4G3-Peyq5dkLJTe2f-3Bj9KuWZjsH2A9Qq5PO5BRLrVfWGPnI3eQu8RkTPgyeUf9ZOWY9JbptVJy9LceAyHRn-O0kbzprx88yb82a5dnCR7EDP7n0)

You can read about the original network:

*Source: http://yann.lecun.com/exdb/publis/pdf/lecun-01a.pdf*

Your optimized CUDA implementation of the convolutional layer will be used to perform inference for layers C1 and C3 (shown in red) in the figure above. This leverages the [mini-dnn-cpp](https://github.com/iamhankai/mini-dnn-cpp) (Mini-DNN) framework for implementing the modified LeNet-5.

## Input Data

The network will be tested on the [Fashion MNIST dataset](https://github.com/zalandoresearch/fashion-mnist) which contains 10,000 single channel images each of dimensions 86x86 but we will only use 1000 of these at a time. The output layer consists of 10 nodes, where each node represents the likelihood of the input belonging to one of the 10 classes (T-shirt, dress, sneaker, boot, etc).

## Instructions

This assignment requires you to complete a GPU implementation of the convolutional layer. Performance of the GPU implementation is not important as this assignment is intended to build functionality before optimizing. The only file you need to update to implement the forward convolution is:
`src/layer/custom/new-forward.cu`. To understand which functions within `new-forward.cu` are being called and when, it may be helpful ot refer to `src/layer/conv_cust.cc`.

Again, you are performing the following operation:
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

The `src/layer/custom/new-forward.cu` file contains the code for the programming assignment. There is a Makefile included which compiles it and links it with the libgputk CUDA library automatically. It can be run by typing `make m2` from the PA7 folder. It generates a `m2` output executable.

## How to test

Use the `make run` command to test your program which will run your program on a batch size of 1000 images. This will automatically compile your source (equivilent to executing `make m2` and then running `./m2 1000`).

## Test Output 

You will need to checkout a GPU for this assignment, but please avoid editing while accessing a device. You can accomplish this with:
`launch.sh -g 1 -s -i ucsdets/nvcr-cuda:latest`

The accuracy of your implementation should meet the 0.886 that our implementation does.

## Submission

Submit the src/layer/custom/new-forward.cu file on gradescope.

## Credit

This project is originally from UIUC ECE408 and builds off a number of open source projects including the Fashion MNIST dataset, mini-dnn-cpp, and the Eigen project.
