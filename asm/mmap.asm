; Defines the memory map of the conceptual MC8-based computer meant for
; implementation in Minecraft. This memory map defines a small ROM bank
; for a serial bootloader, memory-mapped serial IO registers, and a 
; large RAM bank for the remainder of the address space. This is an
; include file that should be included in MC8 assembly files which target
; this computer architecture.

#once
#include "mc8.asm"

#bankdef rom {
    #addr 0
    #size 32
    #outp 0
    #fill
}

#bankdef ram {
    #addr 0
    #size 222
    ; Non-writable
}

SERIAL_OUTPUT = 0x00
SERIAL_CTRL   = 0xFE
SERIAL_INPUT  = 0xFF

#ruledef serial_interface {
    in a  => asm { load a, [SERIAL_INPUT]   }
    in b  => asm { load b, [SERIAL_INPUT]   }
    out a => asm { store [SERIAL_OUTPUT], a }
    out b => asm { store [SERIAL_OUTPUT], b }
}

#bank rom
