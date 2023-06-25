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

HALT_EMULATOR = 0
SERIAL_CONTROL = 0xFE
SERIAL_DATA = 0xFF

#ruledef emu_ctrl {
    halt => asm {
        store [HALT_EMULATOR], a
    }
    in a => asm {
        load a, [SERIAL_DATA]
    }
    in b => asm {
        load b, [SERIAL_DATA]
    }
    out a => asm {
        store [SERIAL_DATA], a
    }
    out b => asm {
        store [SERIAL_DATA], b
    }
}

#bank rom
