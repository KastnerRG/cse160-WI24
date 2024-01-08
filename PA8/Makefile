m2:		m2.o custom
		nvcc -o m2 -lm -lcuda -lrt m2.o ece408net.o src/network.o src/mnist.o src/layer/*.o src/loss/*.o src/layer/custom/*.o ../libgputk/lib/libgputk.a -I./ 

m2.o:		m2.cc
		nvcc --compile m2.cc -I ../libgputk/ -I./

ece408net.o:    ece408net.cc
		nvcc --compile ece408net.cc -I ../libgputk/ -I./

network.o:	src/network.cc
		nvcc --compile src/network.cc -o src/network.o -I ../libgputk/ -I./

mnist.o:	src/mnist.cc
		nvcc --compile src/mnist.cc -o src/mnist.o -I ../libgputk/ -I./

layer:		src/layer/conv.cc src/layer/ave_pooling.cc src/layer/conv_cpu.cc src/layer/conv_cust.cc src/layer/fully_connected.cc src/layer/max_pooling.cc src/layer/relu.cc src/layer/sigmoid.cc src/layer/softmax.cc 
		nvcc --compile src/layer/ave_pooling.cc -o src/layer/ave_pooling.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv.cc -o src/layer/conv.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv_cpu.cc -o src/layer/conv_cpu.o -I ../libgputk/ -I./
		nvcc --compile src/layer/conv_cust.cc -o src/layer/conv_cust.o -I ../libgputk/ -I./
		nvcc --compile src/layer/fully_connected.cc -o src/layer/fully_connected.o -I ../libgputk/ -I./
		nvcc --compile src/layer/max_pooling.cc -o src/layer/max_pooling.o -I ../libgputk/ -I./
		nvcc --compile src/layer/relu.cc -o src/layer/relu.o -I ../libgputk/ -I./
		nvcc --compile src/layer/sigmoid.cc -o src/layer/sigmoid.o -I ../libgputk/ -I./
		nvcc --compile src/layer/softmax.cc -o src/layer/softmax.o -I ../libgputk/ -I./

custom:
		nvcc --compile src/layer/custom/cpu-new-forward.cc -o src/layer/custom/cpu-new-forward.o -I ../libgputk/ -I./
		nvcc --compile src/layer/custom/gpu-utils.cu -o src/layer/custom/gpu-utils.o -I ../libgputk/ -I./
		nvcc --compile src/layer/custom/new-forward.cu -o src/layer/custom/new-forward.o -I ../libgputk/ -I./

loss:           src/loss/cross_entropy_loss.cc src/loss/mse_loss.cc
		nvcc --compile src/loss/cross_entropy_loss.cc -o src/loss/cross_entropy_loss.o -I ../libgputk/ -I./
		nvcc --compile src/loss/mse_loss.cc -o src/loss/mse_loss.o -I ../libgputk/ -I./


clean:
		rm m2
		rm m2.o

run: 		m2
		./m2 1000
