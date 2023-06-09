#include "mc8.asm"

; Calculate the Fibonacci numbers that fit in 8 bits

; Serial output port mapped into memory
SERIAL_PORT = 0xFF

; Variables in RAM
; NEXT_TERM = FIRST_TERM + SECOND_TERM
FIRST_TERM = 0xFE
SECOND_TERM = 0xFD

; Assumption: Registers are cleared to zero at "load-time" (pre run-time)
; A = 0, B = 0

; FIRST_TERM = 0
; SECOND_TERM = 1
store [FIRST_TERM], a
addc a, 1
store [SECOND_TERM], a

clear

loop:
    ; A = FIRST_TERM + SECOND_TERM
    load a, [FIRST_TERM]
    load b, [SECOND_TERM]
    addc a, b

    jmp.c halt
    store [SERIAL_PORT], a

    ; FIRST_TERM = SECOND_TERM
    ; SECOND_TERM = NEXT_TERM
    store [FIRST_TERM], b
    store [SECOND_TERM], a

    jmp.nc loop

halt:
    jmp.c halt