#ifndef MC8_H
#define MC8_H

#include <stdint.h>

#include "bus.h"

// TODO: Namespace should be renamed to cpu to generalize emulation system.

/// @brief Contains declarations relating to 8-bit CPU operation.
namespace mc8 {

    // TODO: Struct should be renamed to cpu to generalize emulation system.

    /// @brief An 8-bit CPU and associated state.
    typedef struct mc8 mc8;

    // TODO: Would using integers give implementors flexibility in error definition?

    /// @brief Error cases over life cycle of CPU operation.
    enum CpuError {
        NO_ERROR,
        CPU_ALLOCATION_FAILURE,
        NULL_BUS,
    };

    // TODO: Accept char* as parameter to give implementors flexibility in representation.

    /// @brief Return a C-string representation of the provided CpuError
    /// @param[in] error CpuError to represent.
    /// @return C-string representation of error.
    const char *error_repr(const CpuError error);

    /// @brief Create and initialize a CPU using provided bus.
    /// @param[in] bus Bus state for CPU to interact with.
    /// @param[out] error Output parameter representing error state of initialization.
    /// @return Initialized cpu.
    /// @note CPU takes ownership of bus.
    /// @note CPU is dynamically allocated and owned by the caller.
    mc8* create(bus::Bus *const bus, CpuError *const error);

    /// @brief Destruct CPU.
    /// @param[in] cpu CPU to release.
    /// @note CPU is unusable following call to release.
    void release(mc8 *const cpu);

    // TODO: Run needs a way to signify reason for finishing. Via BusError?

    /// @brief Execute CPU instructions.
    /// @param[in] cpu CPU to execute via.
    /// @param[in] max_cycles Maximum number of instructions to execute. 
    /// @return Actual number of instructions executed.
    uint32_t run(mc8 *const cpu, uint32_t max_cycles);
}

#endif
