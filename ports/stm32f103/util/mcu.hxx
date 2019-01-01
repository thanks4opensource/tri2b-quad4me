#ifndef MCU_HXX
#define MCU_HXX

#include <stdint.h>

#include <mcu_config.hxx>

namespace baresil {
namespace stm32f10_12357_xx {
namespace mcu {

void    init();



// can't specify mcu_config::MAIN_CLOCK_HZ because intermediate value
// overflows 32 bits, and dividing early loses precision

static inline constexpr uint32_t microseconds_to_clocks(
const uint32_t  microseconds)
{
    return   microseconds * mcu_config::MAIN_CLOCK_MHZ;
}

static inline constexpr uint32_t milliseconds_to_clocks(
const uint32_t  milliseconds)
{
    return milliseconds * mcu_config::MAIN_CLOCK_KHZ;
}

static inline constexpr uint32_t seconds_to_clocks(
const uint32_t  seconds)
{
    return seconds * 1000 * mcu_config::MAIN_CLOCK_KHZ;
}


} // namespace mcu
} // namespace stm32f10_12357_xx
} // namespace baresil

#endif   // ifndef MCU_HXX
