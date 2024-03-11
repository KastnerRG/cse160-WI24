m1:		m1.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel custom.sentinel
		nvcc -o m1 -lm -lcuda -lrt m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./

m2:		m2.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel custom.sentinel
		nvcc -o m2 -lm -lcuda -lrt m2.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

debug:	debug_m1

debug_m1: m1.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel src/layer/custom/cpu-new-forward.cc src/layer/custom/gpu-utils.cu src/layer/custom/new-forward.cu
		rm custom.sentinel || true
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query` -o m1 -lm -lcuda -lrt m1.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

debug_m2: m1.o ece408net.o src/network.o src/mnist.o layer.sentinel loss.sentinel src/layer/custom/cpu-new-forward.cc src/layer/custom/gpu-utils.cu src/layer/custom/new-forward.cu
		rm custom.sentinel || true
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query`  --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
		nvcc -g -G -gencode arch=compute_`/usr/local/cuda/bin/__nvcc_device_query`,code=sm_`/usr/local/cuda/bin/__nvcc_device_query` -o m2 -lm -lcuda -lrt m2.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

m2.o:		m2.cc
		nvcc --compile m2.cc -I ../libgputk/ -I./

m1.o:		m1.cc
		nvcc --compile m1.cc -I ../libgputk/ -I./

ece408net.o:    ece408net.cc
		nvcc --compile ece408net.cc -I ../libgputk/ -I./

src/network.o:	src/network.cc
		nvcc --compile src/network.cc -o src/network.o -I ../libgputk/ -I./

src/mnist.o:	src/mnist.cc
		nvcc --compile src/mnist.cc -o src/mnist.o -I ../libgputk/ -I./

layer.sentinel:		src/layer/conv.cc src/layer/ave_pooling.cc src/layer/conv_cpu.cc src/layer/conv_cust.cc src/layer/fully_connected.cc src/layer/max_pooling.cc src/layer/relu.cc src/layer/sigmoid.cc src/layer/softmax.cc 
		nvcc --compile src/layer/ave_pooling.cc -o src/layer/ave_pooling.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv.cc -o src/layer/conv.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv_cpu.cc -o src/layer/conv_cpu.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv_cust.cc -o src/layer/conv_cust.o -I ../libgputk/ -I./
		nvcc --compile src/layer/fully_connected.cc -o src/layer/fully_connected.o -I ../libgputk/ -I./
		nvcc --compile src/layer/max_pooling.cc -o src/layer/max_pooling.o -I ../libgputk/ -I./
		nvcc --compile src/layer/relu.cc -o src/layer/relu.o -I ../libgputk/ -I./
		nvcc --compile src/layer/sigmoid.cc -o src/layer/sigmoid.o -I ../libgputk/ -I./
		nvcc --compile src/layer/softmax.cc -o src/layer/softmax.o -I ../libgputk/ -I./
		touch layer.sentinel

custom.sentinel: src/layer/custom/cpu-new-forward.cc src/layer/custom/gpu-utils.cu src/layer/custom/new-forward.cu
		nvcc --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
		nvcc --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
		nvcc --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./
		touch custom.sentinel

loss.sentinel:           src/loss/cross_entropy_loss.cc src/loss/mse_loss.cc
		nvcc --compile src/loss/cross_entropy_loss.cc -o src/loss/cross_entropy_loss.o -I ../libgputk/ -I./
		nvcc --compile src/loss/mse_loss.cc -o src/loss/mse_loss.o -I ../libgputk/ -I./
		touch loss.sentinel


clean:
		rm m1
		rm m1.o
		find . -name "*.o" -type f -delete
		rm *.sentinel

run: 		m1
		./m1 1000
