#! /opt/conda/bin/python
import os
from subprocess import Popen, PIPE

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

SUPPORTED_GPU = "NVIDIA GeForce GTX 1080 Ti"

def check_gpumodel():
    if not os.path.exists("/proc/driver/nvidia/gpus/"):
        return

    dirs = os.listdir("/proc/driver/nvidia/gpus/")
    dirs.sort()

    with open(os.path.join("/proc/driver/nvidia/gpus/", f"{dirs[0]}/information"), "r") as f:
        model = f.readline()

    model = model[len("Model:"):].strip()

    if model != SUPPORTED_GPU:
        print(f"{bcolors.WARNING}Warning: You not on the same GPU that we will be testing against." +
              f"  You are on using a \"{model}\" and we are testing on a \"{SUPPORTED_GPU}\"\n.{bcolors.ENDC}")
        
def get_executiontime():
    results = []
    for _ in range(10):
        process = Popen([
            "/usr/local/cuda/bin/nvprof",
            "-u",
            "ms",
            "./solution",
            "-e",
            "./Dataset/10/output.ppm",
            "-i",
            "./Dataset/10/input0.ppm,./Dataset/10/input1.raw",
            "-t",
            "image"], stderr=PIPE)
        
        _, err = process.communicate()

        for line in err.decode().splitlines():
            for seg in line.split():
                if 'convolution' in seg:
                    prof = line.split()
                    results.append(float(prof[3]))

    return sum(results) / float(len(results))

def main():
    check_gpumodel()
    execution_time = get_executiontime()

    print(f"\nYour time was {bcolors.OKGREEN}{execution_time:.2f} ms{bcolors.ENDC}.")

if __name__ == '__main__':
    main()
