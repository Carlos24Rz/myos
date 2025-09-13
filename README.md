# myos — Minimal 32-bit x86 (i386) Multiboot Kernel

A tiny, freestanding kernel for x86 (i386) that boots via GRUB using the Multiboot v1 specification. It includes:
- A multiboot-compliant assembly loader that sets up a stack and jumps into C
- A freestanding C entrypoint (kmain)
- A linker script that places sections at 1 MiB with 4 KiB alignment
- A GRUB bootable ISO image and a QEMU run target

This is a learning-oriented skeleton following common OSDev practices.


## Quick Start

Common make targets:
- `make` — build `kernel.elf`
- `make myos.iso` — build a GRUB-bootable ISO at `myos.iso`
- `make run` — boot the ISO in QEMU (i386)
- `make clean` — remove build artifacts

Example:
```bash
make myos.iso
make run
```

QEMU will boot GRUB, which will load and launch your kernel.


## Project Layout

- `boot.s` — Multiboot v1 header and loader:
  - Defines the magic, flags, checksum
  - Reserves a 4 KiB stack, sets ESP
  - Calls `kmain` and then loops
- `kmain.c` — The C kernel entrypoint (`void kmain(void)`)
- `linker.ld` — Linker script:
  - Entry symbol `loader`
  - Load address `0x00100000` (1 MiB)
  - 4 KiB-aligned `.text`, `.rodata`, `.data`, `.bss`
- `isodir/boot/grub/grub.cfg` — GRUB menuentry that multiboots `/boot/kernel.elf`
- `Makefile` — Build, ISO creation, and run targets


## Requirements

You need a cross-compiling toolchain and boot utilities suitable for bare-metal:

- i686-elf binutils and gcc:
  - `i686-elf-gcc`
  - `i686-elf-as`
- GRUB utilities to build an ISO:
  - The Makefile expects `i686-elf-grub-mkrescue`
  - On many systems this tool is named `grub-mkrescue` (no prefix)
- xorriso (required by `grub-mkrescue`)
- QEMU (for running): `qemu-system-i386`
- make