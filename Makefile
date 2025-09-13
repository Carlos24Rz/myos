CC = i686-elf-gcc
CFLAGS = -ffreestanding -Wall -Wextra -Werror
AS = i686-elf-as
LDFLAGS = -ffreestanding -nostdlib -lgcc

OBJECTS = boot.o kmain.o

all: kernel.elf

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

myos.iso: kernel.elf
	cp kernel.elf isodir/boot
	i686-elf-grub-mkrescue -o myos.iso isodir

kernel.elf: $(OBJECTS) linker.ld
	$(CC) -T linker.ld -o kernel.elf $(LDFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -rf kernel.elf *.o myos.iso
