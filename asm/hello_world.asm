#include "mc8.asm"
#include "mmap.asm"
#include "emu.asm"

zero
clear
addc a, hello_world

print_loop:
    cmp a, hello_world_end
    jmp.z end
    load b, [a]
    out b
    addc a, 1
    jmp.nc print_loop

end:
    halt

hello_world: #d "Hello, World!\n"
hello_world_end:
