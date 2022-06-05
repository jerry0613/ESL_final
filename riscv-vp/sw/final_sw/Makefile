all : main.c bootstrap.S
	riscv32-unknown-elf-g++ -std=c++14 main.c bootstrap.S -o main -lm -nostartfiles -march=rv32ima -mabi=ilp32
	
origin: all
	../../vp/build/bin/riscv-vp-final --intercept-syscalls main

pipe: all
	../../vp/build/bin/riscv-vp-final-pipe --intercept-syscalls main

parallel: all
	../../vp/build/bin/riscv-vp-final-parallel --intercept-syscalls main

clean:
	rm -f main
