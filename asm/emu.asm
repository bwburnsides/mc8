; Defines the interface between the reference emulator and emulated MC8
; machines. This is an include file meant for use in source targeting
; the reference emulator.

#once
#include "mc8.asm"

HALT_EMULATOR = 0x01

#ruledef emulator_control {
    halt => asm { store [HALT_EMULATOR], a }
}
