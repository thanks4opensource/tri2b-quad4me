#include <LPC8xx.h>

#include <bitops.hxx>
#include <lpc8xx.hxx>

#include <mcu.hxx>

#include <mcu_config.hxx>   // build-specific configuration

using namespace bitops;
using namespace mcu_config;


namespace baresil::lpc8xx::mcu {

void init()
{

    LPC_SYSCON->MAINCLKSEL   = sys_con::main_clk_sel::IRC_OSCILLATOR;
    LPC_SYSCON->MAINCLKUEN   = sys_con::main_clk_uen::NO_CHANGE     ;
    LPC_SYSCON->MAINCLKUEN   = sys_con::main_clk_uen::UPDATE_SOURCE ;
    LPC_SYSCON->SYSAHBCLKDIV = MAIN_CLOCK_DIVISOR;

    const uint32_t  p_reset_ctrl_bits =   0
#ifdef MCU_GPIO_RESET
                                        | sys_con::p_reset_ctrl    ::GPIO_BIT
#endif
#ifdef MCU_UART0_RESET
                                        | sys_con::p_reset_ctrl    ::UART0_BIT
#endif
#ifdef MCU_UART1_RESET
                                        | sys_con::p_reset_ctrl    ::UART1_BIT
#endif
#ifdef MCU_UART2_RESET
                                        | sys_con::p_reset_ctrl    ::UART2_BIT
#endif
#ifdef MCU_SPI0_RESET
                                        | sys_con::p_reset_ctrl    ::SPI0_BIT
#endif
#ifdef MCU_SPI1_RESET
                                        | sys_con::p_reset_ctrl    ::SPI1_BIT
#endif
#ifdef MCU_I2C0_RESET
                                        | sys_con::p_reset_ctrl    ::I2C0_BIT
#endif
#ifdef MCU_I2C1_RESET
                                        | sys_con::p_reset_ctrl    ::I2C1_BIT
#endif
#ifdef MCU_I2C2_RESET
                                        | sys_con::p_reset_ctrl    ::I2C2_BIT
#endif
#ifdef MCU_I2C3_RESET
                                        | sys_con::p_reset_ctrl    ::I2C3_BIT
#endif
#ifdef MCU_SCT_RESET
                                        | sys_con::p_reset_ctrl    ::SCT_BIT
#endif
#ifdef MCU_MRT_RESET
                                        | sys_con::p_reset_ctrl    ::MRT_BIT
#endif
#ifdef MCU_DMA_RESET
                                        | sys_con::p_reset_ctrl    ::DMA_BIT
#endif
                                                                              ,
                    ahb_clk_ctrl_bits =   0
#ifdef MCU_GPIO_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::GPIO_BIT
#endif
#ifdef MCU_UART0_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::UART0_BIT
#endif
#ifdef MCU_UART1_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::UART1_BIT
#endif
#ifdef MCU_UART2_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::UART2_BIT
#endif
#ifdef MCU_SPI0_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::SPI0_BIT
#endif
#ifdef MCU_SPI1_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::SPI1_BIT
#endif
#ifdef MCU_I2C0_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::I2C0_BIT
#endif
#ifdef MCU_I2C1_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::I2C1_BIT
#endif
#ifdef MCU_I2C2_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::I2C2_BIT
#endif
#ifdef MCU_I2C3_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::I2C3_BIT
#endif
#ifdef MCU_SCT_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::SCT_BIT
#endif
#ifdef MCU_MRT_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::MRT_BIT
#endif
#ifdef MCU_DMA_ENABLE
                                        | sys_con::sys_ahb_clk_ctrl::DMA_BIT
#endif
                                                                              ;
    // reset peripherals
    CLR_BITS(LPC_SYSCON->PRESETCTRL, p_reset_ctrl_bits);
    SET_BITS(LPC_SYSCON->PRESETCTRL, p_reset_ctrl_bits);

    // enable peripherals
    SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL, ahb_clk_ctrl_bits);

} // init()

} // namespace baresil::lpc8xx::mcu
