
run:
	@qemu-system-x86_64 -kernel kernel.elf
	@echo Succes!
run-build:
	@qemu-system-x86_64 -cdrom rozos.iso
	@echo Succes!
clean:
	@rm -f *.o kernel.elf
	@rm -f rozos.iso 
	@rm -rf iso
	@echo Succes!
install:
    @sudo apt update
	@sudo apt install grub-common grub-pc-bin
	@sudo apt update
	@sudo apt install nasm
	@sudo apt update
	@sudo apt install xorriso
	@sudo apt update
	@sudo apt install qemu-system-x86_64
	@sudo apt update
	@echo Succes!
build:
	@nasm -f elf boot.s -o boot.o
	@gcc -m32 -c kernel.c -o kernel.o -ffreestanding -nostdlib -O0
	@ld -m elf_i386 -T linker.ld -o kernel.elf boot.o kernel.o
	@mkdir -p iso/boot/grub
	@cp kernel.elf iso/boot/
	@cp grub.cfg iso/boot/grub/grub.cfg
	@grub-mkrescue --version
	@grub-mkrescue -o rozos.iso iso/
	@echo Succes!
help:
	@echo make build to build os
	@echo make install all requirements
	@echo make run-build run builded iso
	@echo make run to run kernel