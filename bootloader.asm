#include "mc8.asm"

#bankdef rom {
    #bits 8
    #addr 0x00
    #size 0x3e  ; 62 bytes
    #outp 0x00
}

; Memory Mapped IO ports
UART_CTRL = 0x3e
UART_DATA = 0x3f

; Non-writable RAM bank
#bankdef ram {
    #bits 8
    #addr 0x40
    #addr_end 0x100
}

#bank rom

; Serial Bootloader
zero

; Initialize the write_pointer to the start of RAM
addc a, program_start
store [write_pointer], a

; The first byte received will specify the length
; of the program to read into RAM.
WaitForSizeByte:
    test [UART_CTRL]
    jmp.nc WaitForSizeByte

; Read the byte specifying the program size and store
; it in memory.
SizeAvailable:
    load a, [UART_DATA]
    store [program_size], a

; Wait for program bytes to arrive.
WaitForProgramByte:
    test [UART_CTRL]
    jmp.nc WaitForProgramByte

; Read the program byte, store it at the write_pointer,
; increment the pointer, decrement the program size.
ReadProgramByte:
    load a, [write_pointer]
    load b, [UART_DATA]
    store [a], b

    addc a, 1
    store [write_pointer], a

    load a, [program_size]
    addc a, -1
    store [program_size], a

    jmp.nz WaitForProgramByte

; Entire program has been read in, so jump to start.
DoneReadingProgram:
    jmp.z RAM_START

#bank ram

program_size:  #res 1
write_pointer:  #res 1
program_start:
