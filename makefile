myOs : boot.o kernel.o irq.o vga.o gdt.o idt.o common.o linker.ld  grub.cfg
	i586-elf-gcc -T linker.ld -o ../output/myos.bin -ffreestanding -O2 -nostdlib ../output/boot.o ../output/kernel.o ../output/irq.o ../output/vga.o ../output/gdt.o ../output/gdt_s.o ../output/idt.o ../output/idt_s.o ../output/common.o -lgcc
	mkdir -p ../output/isodir
	mkdir -p ../output/isodir/boot
	cp ../output/myos.bin ../output/isodir/boot/myos.bin
	mkdir -p ../output/isodir/boot/grub
	cp grub.cfg ../output/isodir/boot/grub/grub.cfg
	grub-mkrescue -o ../output/myos.iso ../output/isodir
	qemu-system-i386 -cdrom ../output/myos.iso



boot.o : boot.s
	nasm boot.s -f elf -o ../output/boot.o


kernel.o : kernel.c
	i586-elf-gcc -w -c kernel.c -o ../output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

irq.o : irq.c
	i586-elf-gcc -c irq.c -o  ../output/irq.o -ffreestanding -std=gnu99


vga.o : vga.c
	i586-elf-gcc -c vga.c -o  ../output/vga.o -ffreestanding -std=gnu99



gdt.o : gdt.c gdt_s.s
	i586-elf-gcc -c gdt.c -o  ../output/gdt.o -ffreestanding -std=gnu99
	nasm gdt_s.s -f elf -o ../output/gdt_s.o


idt.o : idt.c idt_s.s
	i586-elf-gcc -c idt.c -o  ../output/idt.o -ffreestanding -std=gnu99
	nasm idt_s.s -f elf -o ../output/idt_s.o

common.o :
	i586-elf-gcc -c common.c -o  ../output/common.o -ffreestanding -std=gnu99