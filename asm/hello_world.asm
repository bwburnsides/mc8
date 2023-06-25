#include "mc8.asm"
#include "mmap.asm"

zero
clear

; Initialize pointer to string
addc a, hello_world

; 14 characters long


print_loop:
    cmp a, hello_world_end
    jmp.z end
    load b, [a]
    store [SERIAL_DATA], b
    addc a, 1
    jmp.nc print_loop

end:
    store [HALT_EMULATOR], b

hello_world: #d "Hello, World!\n"
hello_world_end: #d8 $ - hello_world
