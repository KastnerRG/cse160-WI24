all:	solution

solution:	template.o
		nvcc -o solution -lm -lcuda -lrt template.o ../libgputk/lib/libgputk.a 

template.o:	template.cu
		nvcc --compile template.cu -I ../libgputk/
		
debug:
		nvcc -g -G -gencode arch=compute_60,code=sm_60 --compile template.cu -I ../libgputk/
		nvcc -g -G -gencode arch=compute_60,code=sm_60 -o solution -lm -lcuda -lrt template.o ../libgputk/lib/libgputk.a

dataset_generator:	dataset_generator.o
		nvcc -o dataset_generator -lm -lcuda -lrt dataset_generator.o ../libgputk/lib/libgputk.a 

dataset_generator.o:	dataset_generator.cpp
		nvcc --compile dataset_generator.cpp -I ../libgputk/

clean:
		@-rm -f dataset_generator.o
		@-rm -f dataset_generator
		@-rm -f template.o
		@-rm -f template
		@-rm -f solution.o
		@-rm -f solution
		@-rm -f a.out

run: solution
		@echo "\n"
		@echo "test - 0"
		@./solution -e ./Dataset/0/output.ppm -i ./Dataset/0/input0.ppm,./Dataset/0/input1.raw -t image
		@echo "\n"
		@echo "test - 1"
		@./solution -e ./Dataset/1/output.ppm -i ./Dataset/1/input0.ppm,./Dataset/1/input1.raw -t image
		@echo "\n"
		@echo "test - 2"
		@./solution -e ./Dataset/2/output.ppm -i ./Dataset/2/input0.ppm,./Dataset/2/input1.raw -t image
		@echo "\n"
		@echo "test - 3"
		@./solution -e ./Dataset/3/output.ppm -i ./Dataset/3/input0.ppm,./Dataset/3/input1.raw -t image
		@echo "\n"
		@echo "test - 4"
		@./solution -e ./Dataset/4/output.ppm -i ./Dataset/4/input0.ppm,./Dataset/4/input1.raw -t image
		@echo "\n"
		@echo "test - 5"
		@./solution -e ./Dataset/5/output.ppm -i ./Dataset/5/input0.ppm,./Dataset/5/input1.raw -t image
		@echo "\n"
		@echo "test - 6"
		@./solution -e ./Dataset/6/output.ppm -i ./Dataset/6/input0.ppm,./Dataset/6/input1.raw -t image
		@echo "\n"
