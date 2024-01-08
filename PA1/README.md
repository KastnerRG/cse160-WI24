
# Programming Assignment 1: Device Query

## Objective

The purpose of this programming assignment is to introduce the student to the CUDA hardware resources along with their capabilities. The student is not expected to understand the details of the code, but should understand the process of compiling and running code that will be used in subsequent modules.

## Instructions

The code provided queries the GPU hardware on the system. Do not concentrate on the API calls, but on functions starting with `gpuTK`.

The `gpuTKLog` function logs results, specifically we log the following hardware features:

- GPU card's name
- GPU computation capabilities
- Maximum number of block dimensions
- Maximum number of grid dimensions
- Maximum size of GPU memory
- Amount of constant and share memory
- Warp size

## How to Run

The `template.cu` file contains the code for the programming assignment. There is a Makefile included which compiles it and links it with the libgputk CUDA library automatically. It can be run by typing `make` from the DeviceQuery folder. It generates a `solution` output file.

## Submission

You do not need to submit any code for this assignment. You must answer the questions in Gradescope.
