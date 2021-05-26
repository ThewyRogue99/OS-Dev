C_SOURCES = $(wildcard kernel/*.cpp kernel/*/*.cpp)
HEADERS = $(wildcard kernel/*.h kernel/*/*.h)

OBJ = ${C_SOURCES:.cpp=.o}

all: os-image

run: ../bin/boot/os-image.bin
	qemu-system-x86_64 ../bin/boot/os-image.bin

os-image: kernel.bin boot_sect
	cat ../bin/boot/boot_sect.bin ../bin/boot/kernel.bin > ../bin/boot/os-image.bin

kernel.bin: kernel.elf
	objcopy -R .note -R .comment -S -O binary ../bin/junk/kernel.elf ../bin/boot/kernel.bin

kernel.elf: kernel_entry.o ${OBJ}
	ld -m elf_i386 -o ../bin/junk/kernel.elf -Ttext 0x7e00 $(addprefix ../bin/junk/, $(notdir $^))

kernel_entry.o: kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf -o ../bin/junk/kernel_entry.o

boot_sect: boot_sect.asm
	nasm boot_sect.asm -f bin -o ../bin/boot/boot_sect.bin

%.o : %.cpp ${HEADERS}
	g++ -m32 -fno-asynchronous-unwind-tables -ffreestanding -c $< -o ../bin/junk/$(notdir $@)

clear:
	rm ../bin/junk/*