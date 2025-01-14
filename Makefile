objects = header.o kernel.o
# from https://developer.spacemit.com/documentation?token=BZfYwFfEmiSr7dkintEca9ERn3b
lld_args = -flavor ld -mllvm -cache-line-size=64 -mllvm -prefetch-distance=128 -pie --static --nostdlib
true_linux_path ?= ../../bianu/vmlinuz-6.6.36
# xc_bin should be an absolute path
export PATH := ${xc_bin}:${PATH}

all: build/vmlinuz-6.6.36

build/header.o: header.S
	riscv64-unknown-elf-gcc header.S -c -o build/header.o

build/kernel.o: kernel.c
	riscv64-unknown-elf-gcc kernel.c -c -o build/kernel.o

build/kernel.elf: build/header.o build/kernel.o
	cd build; \
		lld ${lld_args} --script=../kernel.lds ${objects} -o kernel.elf

build/kernel.bin: build/kernel.elf
	cd build; \
		riscv64-unknown-elf-objcopy -O binary kernel.elf kernel.bin

build/vmlinuz-6.6.36: build/kernel.bin
	cd build; \
		gzip -k -n -f -9 kernel.bin; \
		mv kernel.bin.gz vmlinuz-6.6.36

transfer: build/vmlinuz-6.6.36
	cd build; \
		sudo cp vmlinuz-6.6.36 /mnt/vmlinuz-6.6.36; \
		sudo chown root:root /mnt/vmlinuz-6.6.36;

append: build/kernel.bin
	cd build; \
		mv kernel.bin kernel2.bin; \
		cp ${true_linux_path} kernel.bin; \
		cat kernel2.bin >> kernel.bin; \
		rm kernel2.bin

.PHONY: transfer append

