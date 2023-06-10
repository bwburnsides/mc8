#include "mc8.asm"

; Calculate the Fibonacci numbers that fit in 8 bits

; Serial output port mapped into memory
SERIAL_PORT = 0xFF

; Variables in RAM
; NEXT_TERM = FIRST_TERM + SECOND_TERM
FIRST_TERM = 0xFE
SECOND_TERM = 0xFD

zero  ; A = 0, B = 0
store [FIRST_TERM], a
clear
addc a, 1  ; A = 1, B = 0
store [SECOND_TERM], a
; FIRST_TERM = 0
; SECOND_TERM = 1

loop:
    ; A = [SECOND_TERM], B = [SECOND_TERM]
    load a, [SECOND_TERM]
    swap

    ; A = [SECOND_TERM] + [FIRST_TERM]
    addc a, [SECOND_TERM]

    jmp.c halt
    store [SERIAL_PORT], a

    ; FIRST_TERM = SECOND_TERM
    ; SECOND_TERM = NEXT_TERM
    store [FIRST_TERM], b
    store [SECOND_TERM], a

    jmp.nc loop

halt:
    jmp.c halt
