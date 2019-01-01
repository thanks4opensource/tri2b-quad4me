#include <stm32f103xb.h>

#include <core_cm3.h>   // must be after stm32f103x6.h for NVIC_type

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <mcu.hxx>

#include <mcu_config.hxx>   // build-specific configuration

using namespace bitops;
using namespace mcu_config;

namespace baresil {
namespace stm32f10_12357_xx {
namespace mcu {

void init()
{
#ifdef MCU_HPRE
    bitops::CHG_BITS(RCC->CFGR,
                     RCC_CFGR_HPRE_Msk,
                     mcu_config::HPRE_DIV);  // rcc::cfgr::HPRE_DIV_n
#endif

#ifdef MCU_HSI
#if 0   // is on by default
    bitops::SET_BITS(RCC->CR, RCC_CR_HSION);
    while (!bitops::GET_BITS(RCC->CR, RCC_CR_HSIRDY))
        asm("nop");
#endif
#endif

#ifdef MCU_HSE
    bitops::SET_BITS(RCC->CR, RCC_CR_HSEON);
    while (!bitops::GET_BITS(RCC->CR, RCC_CR_HSERDY))
        asm("nop");
#endif

    const uint32_t      apb1enr_bits =   0
#ifdef MCU_TIM2_ENABLE
                                       | RCC_APB1ENR_TIM2EN
#endif
#ifdef MCU_TIM3_ENABLE
                                       | RCC_APB1ENR_TIM3EN
#endif
#ifdef MCU_TIM4_ENABLE
                                       | RCC_APB1ENR_TIM4EN
#endif
#ifdef MCU_I2C1_ENABLE
                                       | RCC_APB1ENR_I2C1EN
#endif
#ifdef MCU_I2C2_ENABLE
                                       | RCC_APB1ENR_I2C2EN
#endif
#ifdef MCU_USART2_ENABLE
                                       | RCC_APB1ENR_USART2EN
#endif
#ifdef MCU_USART3_ENABLE
                                       | RCC_APB1ENR_USART3EN
#endif

                                                            ,
                        apb2enr_bits =   0
#ifdef MCU_IOPA_ENABLE
                                       | RCC_APB2ENR_IOPAEN
#endif
#ifdef MCU_IOPB_ENABLE
                                       | RCC_APB2ENR_IOPBEN
#endif
#ifdef MCU_IOPC_ENABLE
                                       | RCC_APB2ENR_IOPCEN
#endif
#ifdef MCU_AFIO_ENABLE
                                       | RCC_APB2ENR_AFIOEN
#endif
#ifdef MCU_TIM1_ENABLE
                                       | RCC_APB2ENR_TIM1EN)
#endif
#ifdef MCU_USART1_ENABLE
                                       | RCC_APB2ENR_USART1EN
#endif
                                                            ,
                        apb1rst_bits =   0
#ifdef MCU_TIM2_RESET
                                       | RCC_APB1RSTR_TIM2RST
#endif
#ifdef MCU_TIM3_RESET
                                       | RCC_APB1RSTR_TIM3RST
#endif
#ifdef MCU_TIM4_RESET
                                       | RCC_APB1RSTR_TIM4RST
#endif
#ifdef MCU_I2C1_RESET
                                       | RCC_APB1RSTR_I2C1RST
#endif
#ifdef MCU_I2C2_RESET
                                       | RCC_APB1RSTR_I2C2RST
#endif
#ifdef MCU_USART2_RESET
                                       | RCC_APB1RSTR_USART2RST
#endif
#ifdef MCU_USART3_RESET
                                       | RCC_APB1RSTR_USART3RST
#endif
                                                             ,
                    apb2rst_bits =   0
#ifdef MCU_IOPA_RESET
                                       | RCC_APB2RSTR_IOPARST
#endif
#ifdef MCU_IOPB_RESET
                                       | RCC_APB2RSTR_IOPBRST
#endif
#ifdef MCU_IOPC_RESET
                                       | RCC_APB2RSTR_IOPCRST
#endif
#ifdef MCU_AFIO_RESET
                                       | RCC_APB2RSTR_AFIORST
#endif
#ifdef MCU_TIM1_RESET
                                       | RCC_APB2RSTR_TIM1RST
#endif
#ifdef MCU_USART1_RESET
                                       | RCC_APB2RSTR_USART1RST
#endif
                                                             ;

    // no effect and/or optimized out if zero
    SET_BITS(RCC->APB1ENR, apb1enr_bits);

    // no effect and/or optimized out if zero
    SET_BITS(RCC->APB1RSTR, apb1rst_bits);
    CLR_BITS(RCC->APB1RSTR, apb1rst_bits);

    // no effect and/or optimized out if zero
    SET_BITS(RCC->APB2ENR, apb2enr_bits);

    // no effect and/or optimized out if zero
    SET_BITS(RCC->APB2RSTR, apb2rst_bits);
    CLR_BITS(RCC->APB2RSTR, apb2rst_bits);

} // init()

} // namespace mcu
} // namespace stm32f10_12357_xx
} // namespace baresil
