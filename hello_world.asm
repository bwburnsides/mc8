#include "mc8.asm"
#include "memmap.asm"

#bank rom

clear
zero
addc a, hello_world

.print_loop:
    load b, [a]
    out b
    addc a, 1
    swap
    cmp a, 0
    jmp.nz .print_loop
    swap

    ; clear
    ; jmp.nc .print_loop

.end:
    halt

hello_world: #d "Hello, World!\n\0"
