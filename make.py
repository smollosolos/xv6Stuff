import sys
import subprocess

graphics = "-nographic"

if len(sys.argv) == 2 and sys.argv[1] == "-t": #test (Compile)
    subprocess.run(["powershell", "-Command", "docker run --rm -v ${pwd}:/project -w /project -it ghcr.io/francisrstokes/rv-toolchain-docker:main make"])
    subprocess.run(["powershell", "-Command", "docker run --rm -v ${pwd}:/project -w /project -it ghcr.io/francisrstokes/rv-toolchain-docker:main make fs.img"])
elif len(sys.argv) == 2 and sys.argv[1] == "-c": #clean (Remove compiled files and image)
    subprocess.run(["wsl", "make", "clean"])
else: #run (Compile, build image and run QEMU)
    if (len(sys.argv) == 2 and sys.argv[1] == "-g"):
        graphics = ""
    subprocess.run(["powershell", "-Command", "docker run --rm -v ${pwd}:/project -w /project -it ghcr.io/francisrstokes/rv-toolchain-docker:main make"])
    subprocess.run(["powershell", "-Command", "docker run --rm -v ${pwd}:/project -w /project -it ghcr.io/francisrstokes/rv-toolchain-docker:main make fs.img"])
    
    subprocess.run([
    "wsl",
    "qemu-system-riscv64",
    "-machine", "virt",
    "-bios", "none",
    "-kernel", "kernel/kernel",
    "-m", "128M",
    "-smp", "3",
    "-global", "virtio-mmio.force-legacy=false",
    "-drive", "file=fs.img,if=none,format=raw,id=x0",
    "-device", "virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0",
    "-device", "virtio-gpu-device,bus=virtio-mmio-bus.1",
    graphics
])