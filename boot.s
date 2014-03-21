;###############################################################################
;#  title: entry point of kernel
;#  file:  entry.asm
;#  Copyright (c) 2010,2011 Pooya Shahinfar (Pswin)
;###############################################################################

MBOOT_PAGE_ALIGN    equ 1<<0        ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1        ; Provide memory info for kernel
MBOOT_AOUT_KLUDGE  equ 1 << 16      ; GRUB pass symbol table to kernel
MBOOT_HEADER_MAGIC  equ 0x1BADB002  ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.
[EXTERN g_sStack]

%define STACK_SIZE (4 * 1024)


section ._mbootsec
align 4
mboot:
    dd  MBOOT_HEADER_MAGIC      ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS      ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM          ; To ensure that the above values are correct

    dd  mboot                   ; Location of this descriptor
    dd  code                    ; Start of kernel '.text' (code) section.
    dd  bss                     ; End of kernel '.data' section.
    dd  end                     ; End of kernel.
    dd  start                   ; Kernel entry point (initial EIP).


[GLOBAL start]      ; Kernel entry point.
[EXTERN kernel_main]      ; This is the entry point of our C code

section .text
start:
    ;mov [g_pMultiBootInfo],ebx  ; save multiboot structure in the g_pMultiBootInfo
    
    ; setuping new stack
    mov ebp, g_sStack
    mov esp, g_sStack + STACK_SIZE
    
    ; Execute the kernel:
    cli         ; Disable interrupts.
    call kernel_main  ; call our main() function.
    hlt         ; halt system

