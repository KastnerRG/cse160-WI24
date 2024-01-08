all:	solution

solution:	template.o
		nvcc -o solution -lm -lcuda -lrt template.o ../libgputk/lib/libgputk.a 

template.o:	template.cu
		nvcc --compile template.cu -I ../libgputk/
