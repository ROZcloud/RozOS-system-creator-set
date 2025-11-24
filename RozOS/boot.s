section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003  
    dd -(0x1BADB002 + 0x00000003)

section .text
global loader
extern kernel_main

loader:
    call kernel_main
    hlt