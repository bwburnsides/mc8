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

EMU_HALT_ADDR = 0
SERIAL_CTRL_ADDR = 0xFE
SERIAL_DATA_ADDR = 0xFF

#ruledef emu_ctrl {
    halt => asm {
        store [EMU_HALT_ADDR], a
    }
    in a => asm {
        load a, [SERIAL_DATA_ADDR]
    }
    in b => asm {
        load b, [SERIAL_DATA_ADDR]
    }
    out a => asm {
        store [SERIAL_DATA_ADDR], a
    }
    out b => asm {
        store [SERIAL_DATA_ADDR], b
    }
}

#bank rom
