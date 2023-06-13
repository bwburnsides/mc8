#ifndef MC8_H
#define MC8_H

#include <stdint.h>

namespace mc8 {
    typedef struct mc8 mc8;
    typedef uint8_t (*BusRead)(uint8_t addr);
    typedef bool (*BusWrite)(uint8_t addr, uint8_t data);

    mc8* create(BusRead read, BusWrite write);
    void release(mc8* cpu);
    size_t run(mc8* cpu);
};

#endif
