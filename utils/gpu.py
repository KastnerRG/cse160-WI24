from subprocess import Popen, PIPE

class GPU:
    @property
    def model(self) -> str:
        process = Popen(["/usr/local/nvidia/bin/nvidia-smi", "--query"], stdout=PIPE, text=True)
        out, _ = process.communicate()

        for line in out.splitlines():
            line = line.strip()
            
            if not line.startswith("Product Name"):
                continue

            return " ".join([l for l in line.split(" ") if l.strip()][3:])


if __name__ == "__main__":
    gpu = GPU()
    print(gpu.model)