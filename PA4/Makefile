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
		@./solution -e ./Dataset/0/output.raw -i ./Dataset/0/input0.raw,./Dataset/0/input1.raw -t matrix
		@echo "\n"
		@echo "test - 1"
		@./solution -e ./Dataset/1/output.raw -i ./Dataset/1/input0.raw,./Dataset/1/input1.raw -t matrix
		@echo "\n"
		@echo "test - 2"
		@./solution -e ./Dataset/2/output.raw -i ./Dataset/2/input0.raw,./Dataset/2/input1.raw -t matrix
		@echo "\n"
		@echo "test - 3"
		@./solution -e ./Dataset/3/output.raw -i ./Dataset/3/input0.raw,./Dataset/3/input1.raw -t matrix
		@echo "\n"
		@echo "test - 4"
		@./solution -e ./Dataset/4/output.raw -i ./Dataset/4/input0.raw,./Dataset/4/input1.raw -t matrix
		@echo "\n"
		@echo "test - 5"
		@./solution -e ./Dataset/5/output.raw -i ./Dataset/5/input0.raw,./Dataset/5/input1.raw -t matrix
		@echo "\n"
		@echo "test - 6"
		@./solution -e ./Dataset/6/output.raw -i ./Dataset/6/input0.raw,./Dataset/6/input1.raw -t matrix
		@echo "\n"
		@echo "test - 7"
		@./solution -e ./Dataset/7/output.raw -i ./Dataset/7/input0.raw,./Dataset/7/input1.raw -t matrix
		@echo "\n"
		@echo "test - 8"
		@./solution -e ./Dataset/8/output.raw -i ./Dataset/8/input0.raw,./Dataset/8/input1.raw -t matrix
		@echo "\n"

run_big: solution
		@echo "\n"
		@echo "test - 9"
		@./solution -e ./Dataset/9/output.raw -i ./Dataset/9/input0.raw,./Dataset/9/input1.raw -t matrix
		@echo "\n"