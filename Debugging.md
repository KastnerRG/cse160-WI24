# Tips for debugging


Hi All,
We are posting some tips which can help you debug your code, there are few important points where your code can have bugs:

1. The total grid / threadblock is not mapped correctly to cover the entire input/output
2. Inside the CUDA kernel, you are not loading the elements from the global memory to shared memory or registers: check if each thread loads the expected element correctly and if loading multiple values then whether the indices of the multiple elements is correct or not
3. The loop / main body of the thread is not functionally correct
4. Your conditions are not correct (check the if statements thoroughly, check the boundary conditions of loops and the threads / thread blocks )
5. Try to dry run the code with example values to see how it proceeds
6. Use the Debuggers (gdb or Nsight) much more to narrow down which part of the code is causing the problem. If you simply start with a particular thread and follow its execution till the end and if it executes all statements correctly then that means the functional part of the code is correct. Then the issue is most likely to be in conditions or boundary cases so try to double check those.
7. If you face an error like:
   > ### "warning: Cuda API error detected: cudaLaunchKernel returned (0xd1)"
   This happened to a lot of students while launching the gdb or Nsight. 
   It seems that removing the flags that specify the gpu architecture and gencode from make debug, and only leaving the important debug flags (-g -G) are a way to get around this error.
8. Making custom and smaller datasets for debugging:
   Find the smallest dataset that fails for your implementation and go from there.
   To make a dataset, all you have to do is add a `generate()` function call in the main() function of dataset_generator.cpp. 
   The specifics and arguments of the `generate()` function depends on the input format so you can look that up based on which PA it is.
   Then, call 
   ```
   make dataset_generator
   ./dataset_generator
   ```
   and the new dataset should appear in the 'Dataset' folder.

9. I am hopeful if you are diligent in these steps, most of your issues are likely to be solved.


Also if you come to the instructors simply saying the code is not working, it is not very likely that we can pinpoint the exact cause as that would require us to read through every line of code carefully and may also require us to run the debugger. So we strongly encourage you to do some basic debugging with debugger to ensure that you are able to identify the cause of the problem.
Believe us, learning how to debug your code is a very important skill that will help you a lot when working on larger projects later on :)
