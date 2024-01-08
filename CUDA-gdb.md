# STEPS TO INVOKE CUDA-GDB


1. Recompile the program and create new binary 

```
nvcc -g -G -gencode arch=compute_60,code=sm_60 --compile template.cu -I ../libgputk/
nvcc -g -G -gencode arch=compute_60,code=sm_60 -o solution -lm -lcuda -lrt template.o ../libgputk/lib/libgputk.a

```

2. Run GDB
```
cuda-gdb solution
```

3. Set args
```
set args -e ./Dataset/9/output.raw -i ./Dataset/9/input0.raw,./Dataset/9/input1.raw -t vector
// This sets the input and output from Dataset/9/ directory
```

4. Run the solution
```
run
```


OTHER THINGS TO ADD IN DEBUGGING

Create a breakpoint
```
break vecAdd
```
or
```
break template.cu: 100 // to add at line 100
```

Execute Next line
```
n
```

Print individual variables
```
print index
```

Change the thread index
```
print index
cuda thread 9
print index
```

Print the kernel block thread
```
cuda kernel block thread
```

Change the thread, block
```
cuda kernel 1 block 1,0,0 thread 3,0,0
```

Check the value of the variables in run-time
```
x/20f in1
x/100f out //to print the outputs
```

Printing individual elements in array
```
print in1[40]
print in2[40]
print out[40]
```


For more resources, check the Discussion section held on Jan 27, 2023. This is a short presentation on CUDA-GDB which will also help:
https://developer.download.nvidia.com/GTC/PDF/1062_Satoor.pdf
