from abc import ABC, abstractmethod
from subprocess import Popen, PIPE
from typing import List

class NVProfBase(ABC):
    def __init__(self) -> None:
        super().__init__()

    @abstractmethod
    def profile(self) -> List[str]:
        raise NotImplementedError

    def __call__(self) -> float:
        lines = self.profile()

        entered_context = False
        time = 0.0
        for line in lines:
            line = line.strip()
            if line.startswith("GPU activities:"):
                entered_context = True
                line = line.replace("GPU activities:", "").strip()

            if not entered_context:
                continue

            if line.startswith("API calls:"):
                break

            if "[CUDA " in line:
                continue

            line = line.replace(", ", ",")
            time_str = [l.strip() for l in line.split(" ") if l.strip()][1]
            time_unit = time_str[-2:]
            
            curr_time = float(time_str[:-2])
            if time_unit == "ms":
                curr_time *= 1000

            time += curr_time

        return time / 1000
    
class NVProfExecutable(NVProfBase):
    def __init__(self, args: List[str]) -> None:
        super().__init__()

        self._args = args

    def profile(self) -> List[str]:
        process = Popen(["/usr/local/cuda/bin/nvprof"] + self._args, stderr=PIPE, text=True)
        _, err = process.communicate()

        return err.splitlines()

class NVProfTest(NVProfBase):
    def __init__(self) -> None:
        super().__init__()

    def profile(self) -> List[str]:
        return """
==139== Profiling application: ./m2 1000
==139== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   75.87%  134.66ms         2  67.332ms  52.117ms  82.547ms  [CUDA memcpy DtoH]
                   14.52%  25.772ms         2  12.886ms  3.8010ms  21.971ms  conv_forward_kernel(float*, float const *, float const *, int, int, int, int, int, int)
                    9.61%  17.054ms         6  2.8424ms  1.2160us  10.435ms  [CUDA memcpy HtoD]
                    0.00%  5.4080us         2  2.7040us  2.6880us  2.7200us  do_not_remove_this_kernel(void)
                    0.00%  4.9920us         2  2.4960us  2.4640us  2.5280us  prefn_marker_kernel(void)
      API calls:   66.53%  371.99ms         8  46.499ms  239.88us  367.16ms  cudaMalloc
                   27.74%  155.10ms         8  19.387ms  47.001us  83.785ms  cudaMemcpy
                    4.61%  25.802ms         6  4.3003ms  5.2320us  21.962ms  cudaDeviceSynchronize
                    0.79%  4.4064ms         8  550.79us  326.17us  1.0101ms  cudaFree
                    0.17%  926.64us       101  9.1740us     120ns  417.43us  cuDeviceGetAttribute
                    0.08%  429.78us         1  429.78us  429.78us  429.78us  cuDeviceTotalMem
                    0.06%  346.69us         6  57.781us  32.435us  109.91us  cudaLaunchKernel
                    0.03%  157.56us         1  157.56us  157.56us  157.56us  cuDeviceGetName
                    0.00%  6.6960us         1  6.6960us  6.6960us  6.6960us  cuDeviceGetPCIBusId
                    0.00%  3.0230us         3  1.0070us     163ns  2.6730us  cuDeviceGetCount
                    0.00%     632ns         2     316ns     135ns     497ns  cuDeviceGet
                    0.00%     228ns         1     228ns     228ns     228ns  cuDeviceGetUuid
""".splitlines()
        

if __name__ == "__main__":
    test = NVProfTest()
    print(test())