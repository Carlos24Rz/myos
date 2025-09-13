# This boot section is discovered by the grub bootloader
# It achieves this by verifying the multiboot 1 magic number
# It also verifies the checksum

.set MAGIC_NUMBER,  0x1BADB002
.set FLAGS,         0x0
.set CHECKSUM,     -MAGIC_NUMBER

# 4 Kb for kernel stack size
.set STACK_SIZE,    4096 

.section .bss
.align 4
# Stack grows from higher addresses to lower addresses
stack_bottom:
  .space STACK_SIZE
stack_top:


.section .text
.align 4           # Multiboot 1 expects a 4 byte alignment
.long MAGIC_NUMBER
.long FLAGS
.long CHECKSUM

.global loader
loader:
  movl $0xCAFEBABE, %eax
  # Set stack pointer for C runtime
  movl $stack_top, %esp

  # Call C kernel etnry poin
  call kmain

.loop:
  jmp  .loop
