
CC = i586-elf-gcc 
ASM = nasm
OPTIONS = -std=gnu99 -ffreestanding  -w  # -Wall -Wextra -O2
INCLUDES = -I includes # Directory for header file
OBJS = main.o module.o # List of objects to be build
CPath = src/
AsmPath= src/asm/
OUTPUT = output/
VPATH = src/asm/:src
myOs : boot.o kernel.o irq.o vga.o gdt.o idt.o common.o linker.ld  grub.cfg
	${CC} ${OPTIONS} -T ${CPath}linker.ld -o ${OUTPUT}myos.bin -ffreestanding -O2 -nostdlib ${OUTPUT}boot.o ${OUTPUT}kernel.o ${OUTPUT}irq.o ${OUTPUT}vga.o ${OUTPUT}gdt.o ${OUTPUT}gdt_s.o ${OUTPUT}idt.o ${OUTPUT}idt_s.o ${OUTPUT}common.o -lgcc
	mkdir -p ${OUTPUT}isodir
	mkdir -p ${OUTPUT}isodir/boot
	cp ${OUTPUT}myos.bin ${OUTPUT}isodir/boot/myos.bin
	mkdir -p ${OUTPUT}isodir/boot/grub
	cp ${CPath}grub.cfg ${OUTPUT}isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${OUTPUT}myos.iso ${OUTPUT}isodir
	qemu-system-i386 -cdrom ${OUTPUT}myos.iso



boot.o : boot.s
	${ASM} ${AsmPath}boot.s -f elf -o ${OUTPUT}boot.o

kernel.o : kernel.c
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}kernel.c -o ${OUTPUT}kernel.o 

irq.o : irq.c
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}irq.c -o  ${OUTPUT}irq.o 

vga.o : vga.c
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}vga.c -o  ${OUTPUT}vga.o 

gdt.o : gdt.c gdt_s.s
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}gdt.c -o  ${OUTPUT}gdt.o
	${ASM} ${AsmPath}gdt_s.s -f elf -o ${OUTPUT}gdt_s.o


idt.o : idt.c idt_s.s            
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}idt.c -o  ${OUTPUT}idt.o
	${ASM} ${AsmPath}idt_s.s -f elf -o ${OUTPUT}idt_s.o

common.o :
	${CC} ${OPTIONS} ${INCLUDES} -c ${CPath}common.c -o  ${OUTPUT}common.o