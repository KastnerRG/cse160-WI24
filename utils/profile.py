#! /opt/conda/bin/python

from argparse import ArgumentParser
from typing import List

from gpu import GPU
from nvprof import NVProfExecutable
from printer import ok, warning

DEFAULT_SUPPORTED_GPU = "NVIDIA GeForce GTX 1080 Ti"

def __check_gpu(supported_gpu: str = DEFAULT_SUPPORTED_GPU):
    gpu = GPU()
    model = gpu.model

    if model != supported_gpu:
        warning("Warning: You not on the same GPU that we will be testing against." +
              f"  You are on using a \"{model}\" and we are testing on a \"{supported_gpu}\"\n.")

if __name__ == "__main__":
    parser = ArgumentParser(
        prog="profile",
        description="Profiles student code using nvprof.",
    )

    parser.add_argument('--gpu', default=DEFAULT_SUPPORTED_GPU) 
    parser.add_argument('--no-warn', action="store_true")
    parser.add_argument('-n', '--num_runs', default=10)
    parser.add_argument('-a', '--args', nargs='+', default=[])

    args = parser.parse_args()

    if not args.no_warn:
        __check_gpu()

    runs: List[float] = []
    for _ in range(args.num_runs):
        prof = NVProfExecutable(args.args)
        runs.append(prof())

    execution_time = sum(runs) / args.num_runs
    print("Time Taken:")
    ok(f"{execution_time:.2f} ms")