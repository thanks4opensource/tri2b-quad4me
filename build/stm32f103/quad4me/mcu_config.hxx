#ifndef MCU_CONFIG_HXX
#define MCU_CONFIG_HXX

#include <stm32f10_12357_xx.hxx>


#if !defined(CLOCK_KHZ) || !defined(CLOCK_MHZ)
#error define CLOCK_KHZ and CLOCK_MHZ in Makefile
#endif


namespace mcu_config {

// main clock

#define MCU_HPRE
#undef MCU_HSI
#undef MCU_HSE

#if !defined(CLOCK_MHZ) || !defined(CLOCK_KHZ)
#error define CLOCK_MHZ and CLOCK_KHZ in Makefile
#endif

static const uint32_t   MAIN_CLOCK_MHZ = CLOCK_MHZ,
                        MAIN_CLOCK_KHZ = CLOCK_KHZ;


#ifdef MCU_HPRE
#ifndef CLOCK_DIV
#error define CLOCK_DIV (as baresil::stm32f10_12357_xx::rcc::cfgr::HPRE_NO_DIV,HPRE_DIV_2...HPRE_DIV_512) in Makefile
#endif   // ifndef CLOCK_DIV
static const uint32_t     HPRE_DIV
                        = baresil::stm32f10_12357_xx::rcc::cfgr::CLOCK_DIV;
#endif   // ifdef MCU_HPRE

#define MCU_IOPB_ENABLE
#define MCU_IOPB_RESET

#ifdef TRIQUAD_INTERRUPTS
#define MCU_AFIO_ENABLE
#define MCU_AFIO_RESET
#endif

#define MCU_TIM2_ENABLE
#define MCU_TIM2_RESET
#define MCU_TIM3_ENABLE
#define MCU_TIM3_RESET
#define MCU_TIM4_ENABLE
#define MCU_TIM4_RESET

} // namespace mcu_config

#endif // ifndef MCU_CONFIG_HXX
