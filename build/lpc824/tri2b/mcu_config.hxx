#ifndef MCU_CONFIG_HXX
#define MCU_CONFIG_HXX

#if !defined(CLOCK_KHZ) || !defined(CLOCK_MHZ) || !defined(CLOCK_DIV)
#error define CLOCK_KHZ, CLOCK_MHZ, and CLOCK_DIV in Makefile
#endif


namespace mcu_config {

// main clock
static const uint32_t   MAIN_CLOCK_MHZ     = CLOCK_MHZ,
                        MAIN_CLOCK_KHZ     = CLOCK_KHZ,
                        MAIN_CLOCK_DIVISOR = CLOCK_DIV;

#define MCU_GPIO_RESET
#define MCU_GPIO_ENABLE

#define MCU_SCT_RESET
#define MCU_SCT_ENABLE

#define MCU_MRT_RESET
#define MCU_MRT_ENABLE

} // namespace mcu_config

#endif // ifndef MCU_CONFIG_HXX
