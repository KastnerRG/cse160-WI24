
# Programming Assignment 3: Matrix Multiplication

## Objective

The purpose of this lab is to get you familiar with mapping computations onto the GPU and multi-dimensional blocks and grids. You will implement matrix multiplication by writing the GPU kernel code as well as the associated host code.

## Instructions

Edit the code in the code tab to perform the following:

- Set the dimensions of the result matrix
- Allocate device memory
- Copy host memory to device
- Initialize thread block and kernel grid dimensions
- Invoke CUDA kernel
- Copy result matrix from device to host
- Free device memory
- Write the CUDA kernel to perform matrix multiplication

Instructions about where to place each part of the code is demarcated by the `//@@` comment lines.

## How to Compile

The `template.cu` file contains the code for the programming assignment. There is a Makefile included which compiles it and links it with the libgputk CUDA library automatically. It can be run by typing `make` from the PA3 folder. It generates a `solution` output file. During development, make sure to run the `make clean` command before running `make`. 

## How to test

Use the `make run` commad to test your program. There are a total of 9 tests on which your program will be evaluated.

## Dataset Generation (optional)

The dataset required to test the program is already generated. If you are interested in how the dataset is generated please refer to the `dataset_generator.cpp` file. You may compile this file using the `make dataset_generator` command and run the executeable using the command `./dataset_generator`. 

## Submission

Submit the template.cu file on gradescope. Preserve the file name while uploading to gradescope.
