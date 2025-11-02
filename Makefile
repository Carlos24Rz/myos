CC = i686-elf-gcc
CFLAGS = -ffreestanding -Wall -Wextra -Werror -MMD -MP
AS = i686-elf-as
LDFLAGS = -ffreestanding -nostdlib -lgcc

MKRESCUE = i686-elf-grub-mkrescue

OBJECTS = boot.o kmain.o io.o framebuffer.o serial.o gdt_c.o gdt_s.o

all: kernel.elf

run: myos.iso
	qemu-system-i386 $(QEMUOPTS) -cdrom myos.iso

myos.iso: kernel.elf
	cp kernel.elf isodir/boot
	$(MKRESCUE) -o myos.iso isodir

kernel.elf: $(OBJECTS) linker.ld
	$(CC) -T linker.ld -o kernel.elf $(LDFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -rf kernel.elf *.o *.d myos.iso

-include $(OBJECTS:.o=.d)
