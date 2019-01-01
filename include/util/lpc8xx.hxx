#ifndef LPC8XX_HXX
#define LPC8XX_HXX

#include <stdint.h>


namespace baresil {

namespace lpc8xx {

// Bit definitions which should have been in NXP, etc LPC8xx.h, etc.

// not bitfields (though tempting) for efficiency of setting multiple at once


namespace nvic {
static const uint8_t    SPI0_POS          =  0,
                        SPI1_POS          =  1,
                        UART0_POS         =  3,
                        UART1_POS         =  4,
                        UART2_POS         =  5,
                        RESERVED1_POS     =  6,
#ifdef LPC82X
                        I2C1_POS          =  7,
                        I2C0_POS          =  8,
#endif
#ifdef LPC81X
                        I2C_POS           =  8,
#endif
                        SCT_POS           =  9,
                        MRT_POS           = 10,
                        CMP_POS           = 11,
                        WDT_POS           = 12,
                        BOD_POS           = 13,
                        FLASH_POS         = 14,
                        WKT_POS           = 15,
#ifdef LPC82X
                        ISE_ADC_SEQA_POS  = 16,
                        ISE_ADC_SEQB_POS  = 17,
                        ISE_ADC_THCMP_POS = 18,
                        ISE_ADC_OVR_POS   = 19,
                        ISE_SDMA_POS      = 20,
                        ISE_I2C2_POS      = 21,
                        ISE_I2C3_POS      = 22,
#endif
                        RESERVED11_POS    = 23,
                        PININT0_POS       = 24,
                        PININT1_POS       = 25,
                        PININT2_POS       = 26,
                        PININT3_POS       = 27,
                        PININT4_POS       = 28,
                        PININT5_POS       = 29,
                        PININT6_POS       = 30,
                        PININT7_POS       = 31;
static const uint32_t   SPI0_BIT          = (1 << SPI0_POS         ),
                        SPI1_BIT          = (1 << SPI1_POS         ),
                        UART0_BIT         = (1 << UART0_POS        ),
                        UART1_BIT         = (1 << UART1_POS        ),
                        UART2_BIT         = (1 << UART2_POS        ),
                        RESERVED1_BIT     = (1 << RESERVED1_POS    ),
#ifdef LPC81X
                        I2C_BIT           = (1 << I2C_POS          ),
#endif
#ifdef LPC82X
                        I2C1_BIT          = (1 << I2C1_POS         ),
                        I2C0_BIT          = (1 << I2C0_POS         ),
#endif
                        SCT_BIT           = (1 << SCT_POS          ),
                        MRT_BIT           = (1 << MRT_POS          ),
                        CMP_BIT           = (1 << CMP_POS          ),
                        WDT_BIT           = (1 << WDT_POS          ),
                        BOD_BIT           = (1 << BOD_POS          ),
                        FLASH_BIT         = (1 << FLASH_POS        ),
                        WKT_BIT           = (1 << WKT_POS          ),
#ifdef LPC82X
                        ISE_ADC_SEQA_BIT  = (1 << ISE_ADC_SEQA_POS ),
                        ISE_ADC_SEQB_BIT  = (1 << ISE_ADC_SEQB_POS ),
                        ISE_ADC_THCMP_BIT = (1 << ISE_ADC_THCMP_POS),
                        ISE_ADC_OVR_BIT   = (1 << ISE_ADC_OVR_POS  ),
                        ISE_SDMA_BIT      = (1 << ISE_SDMA_POS     ),
                        ISE_I2C2_BIT      = (1 << ISE_I2C2_POS     ),
                        ISE_I2C3_BIT      = (1 << ISE_I2C3_POS     ),
#endif
                        RESERVED11_BIT    = (1 << RESERVED11_POS   ),
                        PININT0_BIT       = (1 << PININT0_POS      ),
                        PININT1_BIT       = (1 << PININT1_POS      ),
                        PININT2_BIT       = (1 << PININT2_POS      ),
                        PININT3_BIT       = (1 << PININT3_POS      ),
                        PININT4_BIT       = (1 << PININT4_POS      ),
                        PININT5_BIT       = (1 << PININT5_POS      ),
                        PININT6_BIT       = (1 << PININT6_POS      ),
                        PININT7_BIT       = (1 << PININT7_POS      );
} // namespace nvic


namespace sys_con {
namespace p_reset_ctrl {
static const uint8_t    SPI0_POS    =  0,
                        SPI1_POS    =  1,
                        UARTFRG_POS =  2,
                        UART0_POS   =  3,
                        UART1_POS   =  4,
                        UART2_POS   =  5,
                        I2C_POS     =  6,
                        MRT_POS     =  7,
                        SCT_POS     =  8,
                        WKT_POS     =  9,
                        GPIO_POS    = 10,
                        FLASH_POS   = 11,
                        ACMP_POS    = 12
#ifdef LPC82X
                                        ,
                        I2C0_POS    =  6,
                        I2C1_POS    = 14,
                        I2C2_POS    = 15,
                        I2C3_POS    = 16,
                        ADC_POS     = 24,
                        DMA_POS     = 29
#endif
                        ;
static const uint32_t   SPI0_BIT    = (1 << SPI0_POS   ),
                        SPI1_BIT    = (1 << SPI1_POS   ),
                        UARTFRG_BIT = (1 << UARTFRG_POS),
                        UART0_BIT   = (1 << UART0_POS  ),
                        UART1_BIT   = (1 << UART1_POS  ),
                        UART2_BIT   = (1 << UART2_POS  ),
                        I2C_BIT     = (1 << I2C_POS    ),
                        MRT_BIT     = (1 << MRT_POS    ),
                        SCT_BIT     = (1 << SCT_POS    ),
                        WKT_BIT     = (1 << WKT_POS    ),
                        GPIO_BIT    = (1 << GPIO_POS   ),
                        FLASH_BIT   = (1 << FLASH_POS  ),
                        ACMP_BIT    = (1 << ACMP_POS   )
#ifdef LPC82X
                        ,
                        I2C0_BIT    = (1 << I2C0_POS   ),
                        I2C1_BIT    = (1 << I2C1_POS   ),
                        I2C2_BIT    = (1 << I2C2_POS   ),
                        I2C3_BIT    = (1 << I2C3_POS   ),
                        ADC_BIT     = (1 << ADC_POS    ),
                        DMA_BIT     = (1 << DMA_POS    )
#endif
                        ;
} // namespace p_reset_ctrl

namespace main_clk_sel {
static const uint32_t   IRC_OSCILLATOR      = 0x0,
                        PLL_INPUT           = 0x1,
                        WATCHDOG_OSCILLATOR = 0x2,
                        PLL_OUTPUT          = 0x3;
} // namespace main_clk_sel

namespace main_clk_uen {
static const uint32_t   NO_CHANGE     = 0x0,
                        UPDATE_SOURCE = 0x1;
} // namespace main_clk_uen

namespace sys_ahb_clk_ctrl {
static const uint8_t    SYS_POS      =  0,
                        ROM_POS      =  1,
                        RAM_POS      =  2,
                        FLASHREG_POS =  3,
                        FLASH_POS    =  4,
                        I2C_POS      =  5,
                        GPIO_POS     =  6,
                        SWM_POS      =  7,
                        SCT_POS      =  8,
                        WKT_POS      =  9,
                        MRT_POS      = 10,
                        SPI0_POS     = 11,
                        SPI1_POS     = 12,
                        CRC_POS      = 13,
                        UART0_POS    = 14,
                        UART1_POS    = 15,
                        UART2_POS    = 16,
                        WWDT_POS     = 17,
                        IOCON_POS    = 18,
                        ACMP_POS     = 19
#ifdef LPC82X
                        ,
                        I2C0_POS     =  5,
                        I2C1_POS     = 21,
                        I2C2_POS     = 22,
                        I2C3_POS     = 23,
                        ADC_POS      = 24,
                        MTB_POS      = 26,
                        DMA_POS      = 29
#endif
                        ;
static const uint32_t   SYS_BIT      = (1 << SYS_POS     ),
                        ROM_BIT      = (1 << ROM_POS     ),
                        RAM_BIT      = (1 << RAM_POS     ),
                        FLASHREG_BIT = (1 << FLASHREG_POS),
                        FLASH_BIT    = (1 << FLASH_POS   ),
                        I2C_BIT      = (1 << I2C_POS     ),
                        GPIO_BIT     = (1 << GPIO_POS    ),
                        SWM_BIT      = (1 << SWM_POS     ),
                        SCT_BIT      = (1 << SCT_POS     ),
                        WKT_BIT      = (1 << WKT_POS     ),
                        MRT_BIT      = (1 << MRT_POS     ),
                        SPI0_BIT     = (1 << SPI0_POS    ),
                        SPI1_BIT     = (1 << SPI1_POS    ),
                        CRC_BIT      = (1 << CRC_POS     ),
                        UART0_BIT    = (1 << UART0_POS   ),
                        UART1_BIT    = (1 << UART1_POS   ),
                        UART2_BIT    = (1 << UART2_POS   ),
                        WWDT_BIT     = (1 << WWDT_POS    ),
                        IOCON_BIT    = (1 << IOCON_POS   ),
                        ACMP_BIT     = (1 << ACMP_POS    )
#ifdef LPC82X
                        ,
                        I2C0_BIT     = (1 << I2C0_POS    ),
                        I2C1_BIT     = (1 << I2C1_POS    ),
                        I2C2_BIT     = (1 << I2C2_POS    ),
                        I2C3_BIT     = (1 << I2C3_POS    ),
                        ADC_BIT      = (1 << ADC_POS     ),
                        MTB_BIT      = (1 << MTB_POS     ),
                        DMA_BIT      = (1 << DMA_POS     )
#endif
                        ;
} // namespace sys_ahb_clk_ctrl

namespace pintsel {
    static const uint8_t    INTPIN_POS  = 0;
    static const uint32_t   INTPIN_MASK = (0x3f << INTPIN_POS);
} // namespace pintsel

namespace pdruncfg {    // "power down run configuration"
static const uint8_t    IRCOUT_PD_POS =  0,
                        IRC_PD_POS    =  1,
                        FLASH_PD_POS  =  2,

                        BOD_PD_POS    =  3,
#ifdef LPC82X
                        ADC_PD_POS    =  4,
#endif
                        SYSOSC_PD_POS =  5,
                        WDTOSC_PD_POS =  6,
                        SYSPLL_PD_POS =  7,
                        ACMP_PD_POS   = 15;
// set to 1 to disable, 0 to enable
static const uint32_t   IRCOUT_PD_BIT = (1 << IRCOUT_PD_POS),
                        IRC_PD_BIT    = (1 << IRC_PD_POS   ),
                        FLASH_PD_BIT  = (1 << FLASH_PD_POS ),
                        BOD_PD_BIT    = (1 << BOD_PD_POS   ),
#ifdef LPC82X
                        ADC_PD_BIT    = (1 << ADC_PD_POS   ),
#endif
                        SYSOSC_PD_BIT = (1 << SYSOSC_PD_POS),
                        WDTOSC_PD_BIT = (1 << WDTOSC_PD_POS),
                        SYSPLL_PD_BIT = (1 << SYSPLL_PD_POS),
                        ACMP_PD_BIT   = (1 << ACMP_PD_POS  );
} // namespace pdruncfg
} // namespace sys_con


namespace swm {
// for all _BITS fieltds, have to clear all 8 bits, not just 5

namespace pinassign0 {
static const uint8_t    U0_TXD_O_POS  =  0,
                        U0_RXD_I_POS  =  8,
                        U0_RTS_O_POS  = 16,
                        U0_CTS_I_POS  = 24;
static const uint32_t   U0_TXD_O_MASK = (0xff << U0_TXD_O_POS),
                        U0_RXD_I_MASK = (0xff << U0_RXD_I_POS),
                        U0_RTS_O_MASK = (0xff << U0_RTS_O_POS),
                        U0_CTS_I_MASK = (0xff << U0_CTS_I_POS);
} // namespace pinassign0


namespace pinassign1 {
static const uint8_t    U0_SCLK_IO_POS  =  0,
                        U1_TXD_O_POS    =  8,
                        U1_RXD_I_POS    = 16,
                        U1_RTS_O_POS    = 24;
static const uint32_t   U0_SCLK_IO_MASK = (0xff << U0_SCLK_IO_POS),
                        U1_TXD_O_MASK   = (0xff << U1_TXD_O_POS),
                        U1_RXD_I_MASK   = (0xff << U1_RXD_I_POS),
                        U1_RTS_O_MASK   = (0xff << U1_RTS_O_POS);
} // namespace pinassign1


namespace pinassign3 {
static const uint8_t    SPI0_SCLK_IO_POS  = 24;
static const uint32_t   SPI0_SCLK_IO_MASK = (0xFF << SPI0_SCLK_IO_POS);
} // namespace pinassign3


namespace pinassign4 {
static const uint8_t    SPI0_MOSI_IO_POS  = 0;
static const uint32_t   SPI0_MOSI_IO_MASK = (0xFF << SPI0_MOSI_IO_POS);
} // namespace pinassign4


namespace pinassign5 {
static const uint8_t    SPI1_SCLK_IO_POS  = 16,
                        SPI1_MOSI_IO_POS  = 24;
static const uint32_t   SPI1_SCLK_IO_MASK = (0xFF << SPI1_SCLK_IO_POS),
                        SPI1_MOSI_IO_MASK = (0xFF << SPI1_MOSI_IO_POS);
} // namespace pinassign5


#ifdef LPC82X
namespace pinassign6 {
static const uint8_t    SPI1_MISO_POS   =  0,
                        SPI1_SSEL0_POS  =  8,
                        SPI1_SSEL1_POS  = 16,
                        SCT_PIN0_POS    = 24;
static const uint32_t   SPI1_MISO_MASK  = (0xFF << SPI1_MISO_POS ),
                        SPI1_SSEL0_MASK = (0xFF << SPI1_SSEL0_POS),
                        SPI1_SSEL1_MASK = (0xFF << SPI1_SSEL1_POS),
                        SCT_PIN0_MASK   = (0xFF << SCT_PIN0_POS  );
} // namespace pinassign6
#endif


#ifdef LPC81X
namespace pinassign7 {
static const uint8_t    I2C_SDA_IO_POS    = 24;
static const uint32_t   I2C_SDA_IO_MASK   = (0xFF << I2C_SDA_IO_POS);
} // namespace pinassign7

namespace pinassign8 {
static const uint8_t    I2C_SCL_IO_POS    = 0;
static const uint32_t   I2C_SCL_IO_MASK   = (0xFF << I2C_SCL_IO_POS);
} // namespace pinassign8
#endif // #ifdef LPC81X

#ifdef LPC82X
namespace pinassign9 {
static const uint8_t    I2C1_SDA_IO_POS  =  8,
                        I2C1_SCL_IO_POS  = 16;
static const uint32_t   I2C1_SDA_IO_MASK = (0xFF << I2C1_SDA_IO_POS),
                        I2C1_SCL_IO_MASK = (0xFF << I2C1_SCL_IO_POS);
} // namespace pinassign8
#endif // #ifdef LPC82X


namespace pinenable0 {
#ifdef LPC81X
static const uint8_t    ACMP_I1_EN_POS = 0,
                        ACMP_I2_EN_POS = 1,
                        SWCLK_EN_POS   = 2,
                        SWDIO_EN_POS   = 3,
                        XTALIN_EN_POS  = 4,
                        XTALOUT_EN_POS = 5,
                        RESET_EN_POS   = 6,
                        CLKIN_EN_POS   = 7,
                        VDDCMP_EN_POS  = 8;
static const uint32_t   ACMP_I1_EN_BIT = (1 << ACMP_I1_EN_POS),
                        ACMP_I2_EN_BIT = (1 << ACMP_I2_EN_POS),
                        SWCLK_EN_BIT   = (1 << SWCLK_EN_POS  ),
                        SWDIO_EN_BIT   = (1 << SWDIO_EN_POS  ),
                        XTALIN_EN_BIT  = (1 << XTALIN_EN_POS ),
                        XTALOUT_EN_BIT = (1 << XTALOUT_EN_POS),
                        RESET_EN_BIT   = (1 << RESET_EN_POS  ),
                        CLKIN_EN_BIT   = (1 << CLKIN_EN_POS  ),
                        VDDCMP_EN_BIT  = (1 << VDDCMP_EN_POS );
#endif // #ifdef LPC81X
#ifdef LPC82X
static const uint8_t    ACMP_I1_EN_POS =  0,
                        ACMP_I2_EN_POS =  1,
                        ACMP_I3_EN_POS =  2,
                        ACMP_I4_EN_POS =  3,
                        SWCLK_EN_POS   =  4,
                        SWDIO_EN_POS   =  5,
                        XTALIN_EN_POS  =  6,
                        XTALOUT_EN_POS =  7,
                        RESET_EN_POS   =  8,
                        CLKIN_EN_POS   =  9,
                        VDDCMP_EN_POS  = 10,
                        I2C0_SDA_POS   = 11,
                        I2C0_SCL_POS   = 12,
                        ADC_0_POS      = 13,
                        ADC_1_POS      = 14,
                        ADC_2_POS      = 15,
                        ADC_3_POS      = 16,
                        ADC_4_POS      = 17,
                        ADC_5_POS      = 18,
                        ADC_6_POS      = 19,
                        ADC_7_POS      = 20,
                        ADC_8_POS      = 21,
                        ADC_9_POS      = 22,
                        ADC_10_POS     = 23,
                        ADC_11_POS     = 24;
static const uint32_t   ACMP_I1_EN_BIT = (1 << ACMP_I1_EN_POS),
                        ACMP_I2_EN_BIT = (1 << ACMP_I2_EN_POS),
                        ACMP_I3_EN_BIT = (1 << ACMP_I3_EN_POS),
                        ACMP_I4_EN_BIT = (1 << ACMP_I4_EN_POS),
                        SWCLK_EN_BIT   = (1 << SWCLK_EN_POS  ),
                        SWDIO_EN_BIT   = (1 << SWDIO_EN_POS  ),
                        XTALIN_EN_BIT  = (1 << XTALIN_EN_POS ),
                        XTALOUT_EN_BIT = (1 << XTALOUT_EN_POS),
                        RESET_EN_BIT   = (1 << RESET_EN_POS  ),
                        CLKIN_EN_BIT   = (1 << CLKIN_EN_POS  ),
                        VDDCMP_EN_BIT  = (1 << VDDCMP_EN_POS ),
                        I2C0_SDA_BIT   = (1 << I2C0_SDA_POS  ),
                        I2C0_SCL_BIT   = (1 << I2C0_SCL_POS  ),
                        ADC_0_BIT      = (1 << ADC_0_POS     ),
                        ADC_1_BIT      = (1 << ADC_1_POS     ),
                        ADC_2_BIT      = (1 << ADC_2_POS     ),
                        ADC_3_BIT      = (1 << ADC_3_POS     ),
                        ADC_4_BIT      = (1 << ADC_4_POS     ),
                        ADC_5_BIT      = (1 << ADC_5_POS     ),
                        ADC_6_BIT      = (1 << ADC_6_POS     ),
                        ADC_7_BIT      = (1 << ADC_7_POS     ),
                        ADC_8_BIT      = (1 << ADC_8_POS     ),
                        ADC_9_BIT      = (1 << ADC_9_POS     ),
                        ADC_10_BIT     = (1 << ADC_10_POS    ),
                        ADC_11_BIT     = (1 << ADC_11_POS    );
#endif // #ifdef LPC82X
} // namespace pinenable0 {
} // namespace swm


namespace iocon {
namespace modes {
static const uint8_t    PULL_POS                 =  3,
                        HYSTERESIS_POS           =  5,
                        INVERT_INPUT_POS         =  6,
                        OPEN_DRAIN_POS           = 10,
                        FILTER_MODE_POS          = 11,
                        CLK_DIV_POS              = 13;
static const uint32_t   PULL_MASK                = (0x3 << PULL_POS       ),
                          NO_PULL_BITS           = (0x0 << PULL_POS       ),
                          PULL_DOWN_BITS         = (0x1 << PULL_POS       ),
                          PULL_UP_BITS           = (0x2 << PULL_POS       ),
                          PULL_REPEATER_BITS     = (0x3 << PULL_POS       ),
                        HYSTERESIS_BIT           = (0x1 << HYSTERESIS_POS  ),
                        INVERT_INPUT_BIT         = (0x1 << INVERT_INPUT_POS),
                        OPEN_DRAIN_MASK          = (0x1 << OPEN_DRAIN_POS  ),
                          OPEN_DRAIN_DISABLE_BIT = (0x0 << OPEN_DRAIN_POS  ),
                          OPEN_DRAIN_ENABLE_BIT  = (0x1 << OPEN_DRAIN_POS  ),
                        FILTER_MODE_MASK         = (0x3 << FILTER_MODE_POS ),
                          FILTER_MODE_0_BITS     = (0x0 << FILTER_MODE_POS ),
                          FILTER_MODE_1_BITS     = (0x1 << FILTER_MODE_POS ),
                          FILTER_MODE_2_BITS     = (0x2 << FILTER_MODE_POS ),
                          FILTER_MODE_3_BITS     = (0x3 << FILTER_MODE_POS ),
                        HYSTERESIS_DISABLE_MASK  = (0x0 << HYSTERESIS_POS  ),
                          HYSTERESIS_DISABLE_BIT = (0x0 << HYSTERESIS_POS  ),
                          HYSTERESIS_ENABLE_BIT  = (0x1 << HYSTERESIS_POS  ),
                        CLK_DIV_MASK             = (0x7 << CLK_DIV_POS     ),
                          CLK_DIV_0_BITS         = (0x0 << CLK_DIV_POS     ),
                          CLK_DIV_1_BITS         = (0x1 << CLK_DIV_POS     ),
                          CLK_DIV_2_BITS         = (0x2 << CLK_DIV_POS     ),
                          CLK_DIV_3_BITS         = (0x3 << CLK_DIV_POS     ),
                          CLK_DIV_4_BITS         = (0x4 << CLK_DIV_POS     ),
                          CLK_DIV_5_BITS         = (0x5 << CLK_DIV_POS     ),
                          CLK_DIV_6_BITS         = (0x6 << CLK_DIV_POS     );
} // namespace modes
namespace pio10_11_i2c {
static const uint8_t    INV_POS              =  6,
                        I2C_MODE_POS         =  8,
                        S_MODE_POS           = 11,
                        CLK_DIV_POS          = 13;
static const uint32_t   INV_MASK             = (0x1 << INV_POS     ),
                          INV_NON_BITS       = (0x0 << INV_POS     ),
                          INV_INV_BITS       = (0x1 << INV_POS     ),
                        I2C_MODE_MASK        = (0x3 << I2C_MODE_POS),
                          I2C_STD_FAST_BITS  = (0x0 << I2C_MODE_POS),
                          I2C_GPIO_BITS      = (0x1 << I2C_MODE_POS),
                          I2C_FAST_PLUS_BITS = (0x2 << I2C_MODE_POS),
                        S_MODE_MASK          = (0x3 << S_MODE_POS  ),
                          S_MODE_BYPASS      = (0x0 << S_MODE_POS  ),
                          S_MODE_1_CLK_CYCLS = (0x1 << S_MODE_POS  ),
                          S_MODE_2_CLK_CYCLS = (0x2 << S_MODE_POS  ),
                          S_MODE_3_CLK_CYCLS = (0x3 << S_MODE_POS  ),
                        CLK_DIV_MASK         = (0x7 << CLK_DIV_POS ),
                          CLK_DIV_0_BITS     = (0x0 << CLK_DIV_POS ),
                          CLK_DIV_1_BITS     = (0x1 << CLK_DIV_POS ),
                          CLK_DIV_2_BITS     = (0x2 << CLK_DIV_POS ),
                          CLK_DIV_3_BITS     = (0x3 << CLK_DIV_POS ),
                          CLK_DIV_4_BITS     = (0x4 << CLK_DIV_POS ),
                          CLK_DIV_5_BITS     = (0x5 << CLK_DIV_POS ),
                          CLK_DIV_6_BITS     = (0x6 << CLK_DIV_POS );
} // namespace pio10_11_i2c
} // namespace iocon


namespace gpio {
static const uint8_t    PIN_0_POS  =  0,
                        PIN_1_POS  =  1,
                        PIN_2_POS  =  2,
                        PIN_3_POS  =  3,
                        PIN_4_POS  =  4,
                        PIN_5_POS  =  5,
                        PIN_6_POS  =  6,
                        PIN_7_POS  =  7,
                        PIN_8_POS  =  8,
                        PIN_9_POS  =  9,
                        PIN_10_POS = 10,
                        PIN_11_POS = 11,
                        PIN_12_POS = 12,
                        PIN_13_POS = 13,
                        PIN_14_POS = 14,
                        PIN_15_POS = 15,
                        PIN_16_POS = 16,
                        PIN_17_POS = 17
#ifdef LPC82X
                        ,
                        PIN_18_POS = 18,
                        PIN_19_POS = 19,
                        PIN_20_POS = 20,
                        PIN_21_POS = 21,
                        PIN_22_POS = 22,
                        PIN_23_POS = 23,
                        PIN_24_POS = 24,
                        PIN_25_POS = 25,
                        PIN_26_POS = 26,
                        PIN_27_POS = 27,
                        PIN_28_POS = 28
#endif
                        ;
static const uint32_t   PIN_0_BIT   = (1 << PIN_0_POS ),
                        PIN_1_BIT   = (1 << PIN_1_POS ),
                        PIN_2_BIT   = (1 << PIN_2_POS ),
                        PIN_3_BIT   = (1 << PIN_3_POS ),
                        PIN_4_BIT   = (1 << PIN_4_POS ),
                        PIN_5_BIT   = (1 << PIN_5_POS ),
                        PIN_6_BIT   = (1 << PIN_6_POS ),
                        PIN_7_BIT   = (1 << PIN_7_POS ),
                        PIN_8_BIT   = (1 << PIN_8_POS ),
                        PIN_9_BIT   = (1 << PIN_9_POS ),
                        PIN_10_BIT  = (1 << PIN_10_POS),
                        PIN_11_BIT  = (1 << PIN_11_POS),
                        PIN_12_BIT  = (1 << PIN_12_POS),
                        PIN_13_BIT  = (1 << PIN_13_POS),
                        PIN_14_BIT  = (1 << PIN_14_POS),
                        PIN_15_BIT  = (1 << PIN_15_POS),
                        PIN_16_BIT  = (1 << PIN_16_POS),
                        PIN_17_BIT  = (1 << PIN_17_POS)
#ifdef LPC82X
                        ,
                        PIN_18_BIT  = (1 << PIN_18_POS),
                        PIN_19_BIT  = (1 << PIN_19_POS),
                        PIN_20_BIT  = (1 << PIN_20_POS),
                        PIN_21_BIT  = (1 << PIN_21_POS),
                        PIN_22_BIT  = (1 << PIN_22_POS),
                        PIN_23_BIT  = (1 << PIN_23_POS),
                        PIN_24_BIT  = (1 << PIN_24_POS),
                        PIN_25_BIT  = (1 << PIN_25_POS),
                        PIN_26_BIT  = (1 << PIN_26_POS),
                        PIN_27_BIT  = (1 << PIN_27_POS),
                        PIN_28_BIT  = (1 << PIN_28_POS)
#endif
                        ;
} // namespace gpio


namespace pin_int {
static const uint8_t    PINTSEL_0_POS = 0,
                        PINTSEL_1_POS = 1,
                        PINTSEL_2_POS = 2,
                        PINTSEL_3_POS = 3,
                        PINTSEL_4_POS = 4,
                        PINTSEL_5_POS = 5,
                        PINTSEL_6_POS = 6,
                        PINTSEL_7_POS = 7;
static const uint32_t   PINTSEL_BITS  = 0xff,
                        PINTSEL_0_BIT = 1 << PINTSEL_0_POS,
                        PINTSEL_1_BIT = 1 << PINTSEL_1_POS,
                        PINTSEL_2_BIT = 1 << PINTSEL_2_POS,
                        PINTSEL_3_BIT = 1 << PINTSEL_3_POS,
                        PINTSEL_4_BIT = 1 << PINTSEL_4_POS,
                        PINTSEL_5_BIT = 1 << PINTSEL_5_POS,
                        PINTSEL_6_BIT = 1 << PINTSEL_6_POS,
                        PINTSEL_7_BIT = 1 << PINTSEL_7_POS;

} // namespace pin_int


namespace usart {
namespace cfg {
static const uint8_t    ENABLE_POS       =  0,
                        DATALEN_POS      =  2,
                        PARTITYSEL_POS   =  4,
                        STOPLEN_POS      =  6,
                        CTSEN_POS        =  9,
                        SYNCEN_POS       = 11,
                        CLKPOL_POS       = 12,
                        SYNCMST_POS      = 14,
                        LOOP_POS         = 15,
                        OETA_POS         = 18,
                        AUTOADDR_POS     = 19,
                        OESEL_POS        = 20,
                        OEPOL_POS        = 21,
                        RXPOL_POS        = 22,
                        TXPOL_POS        = 23;
         const uint32_t ENABLE_BIT       = (  1 << ENABLE_POS     ),
                        DATALEN_MASK     = (0x3 << DATALEN_POS    ),
                          DATALEN_7_BITS = (0x0 << DATALEN_POS    ),
                          DATALEN_8_BITS = (0x1 << DATALEN_POS    ),
                          DATALEN_9_BITS = (0x2 << DATALEN_POS    ),
                        PARTITYSEL_MASK  = (0x3 << PARTITYSEL_POS ),
                          PARITYSEL_NONE = (0x0 << PARTITYSEL_POS ),
                          PARITYSEL_EVEN = (0x2 << PARTITYSEL_POS ),
                          PARITYSEL_ODD  = (0x3 << PARTITYSEL_POS ),
                        STOPLEN_BIT      = (     1 << STOPLEN_POS ),
                        CTSEN_BIT        = (     1 << CTSEN_POS   ),
                        SYNCEN_BIT       = (     1 << SYNCEN_POS  ),
                        CLKPOL_BIT       = (     1 << CLKPOL_POS  ),
                        SYNCMST_BIT      = (     1 << SYNCMST_POS ),
                        LOOP_BIT         = (     1 << LOOP_POS    ),
                        OETA_BIT         = (     1 << OETA_POS    ),
                        AUTOADDR_BIT     = (     1 << AUTOADDR_POS),
                        OESEL_BIT        = (     1 << OESEL_POS   ),
                        OEPOL_BIT        = (     1 << OEPOL_POS   ),
                        RXPOL_BIT        = (     1 << RXPOL_POS   ),
                        TXPOL_BIT        = (     1 << TXPOL_POS   );
} // namespace cfg

namespace ctl {
static const uint8_t    TXBRKEN_POS    =  1,
                        ADDRDET_POS    =  2,
                        TXDIS_POS      =  6,
                        CC_POS         =  8,
                        CLRCCONRX_POS  =  9,
                        AUTOBAUD_POS   = 16;
static const uint32_t   TXBRKEN_BIT    = (1 << TXBRKEN_POS  ),
                        ADDRDET_BIT    = (1 << ADDRDET_POS  ),
                        TXDIS_BIT      = (1 << TXDIS_POS    ),
                        CC_BIT         = (1 << CC_POS       ),
                        CLRCCONRX_BIT  = (1 << CLRCCONRX_POS),
                        AUTOBAUD_BIT   = (1 << AUTOBAUD_POS );
} // namespace ctl

namespace stat {
static const uint8_t    RXRDY_POS         =  0,
                        RXIDLE_POS        =  1,
                        TXRDY_POS         =  2,
                        TXIDLE_POS        =  3,
                        CTS_POS           =  4,
                        DELTACTS_POS      =  5,
                        TXDISSTAT_POS     =  6,
                        OVERRUNINT_POS    =  8,
                        RXBRK_POS         = 10,
                        DELTARXBRK_POS    = 11,
                        START_POS         = 12,
                        FRAMERRINT_POS    = 13,
                        PARITYERRINT_POS  = 14,
                        RXNOISEINT_POS    = 15,
                        ABERR_POS         = 16;
static const uint32_t   RXRDY_BIT         = (1 << RXRDY_POS       ),
                        RXIDLE_BIT        = (1 << RXIDLE_POS      ),
                        TXRDY_BIT         = (1 << TXRDY_POS       ),
                        TXIDLE_BIT        = (1 << TXIDLE_POS      ),
                        CTS_BIT           = (1 << CTS_POS         ),
                        DELTACTS_BIT      = (1 << DELTACTS_POS    ),
                        TXDISSTAT_BIT     = (1 << TXDISSTAT_POS   ),
                        OVERRUNINT_BIT    = (1 << OVERRUNINT_POS  ),
                        RXBRK_BIT         = (1 << RXBRK_POS       ),
                        DELTARXBRK_BIT    = (1 << DELTARXBRK_POS  ),
                        START_BIT         = (1 << START_POS       ),
                        FRAMERRINT_BIT    = (1 << FRAMERRINT_POS  ),
                        PARITYERRINT_BIT  = (1 << PARITYERRINT_POS),
                        RXNOISEINT_BIT    = (1 << RXNOISEINT_POS  ),
                        ABERR_BIT         = (1 << ABERR_POS       );
} // namespace stat

namespace intenset {
static const uint8_t    RXRDYEN_POS       =  0,
                        TXRDYEN_POS       =  2,
                        TXIDLEEN_POS      =  3,
                        DELTACTSEN_POS    =  5,
                        TXDISEN_POS       =  6,
                        OVERRUNEN_POS     =  8,
                        DELTARXBRKEN_POS  = 11,
                        STARTEN_POS       = 12,
                        FRAMERREN_POS     = 13,
                        PARITYERREN_POS   = 14,
                        RXNOISEEN_POS     = 15,
                        ABERREN_POS       = 16;
static const uint32_t   RXRDYEN_BIT       = (1 << RXRDYEN_POS     ),
                        TXRDYEN_BIT       = (1 << TXRDYEN_POS     ),
                        TXIDLEEN_BIT      = (1 << TXIDLEEN_POS    ),
                        DELTACTSEN_BIT    = (1 << DELTACTSEN_POS  ),
                        TXDISEN_BIT       = (1 << TXDISEN_POS     ),
                        OVERRUNEN_BIT     = (1 << OVERRUNEN_POS   ),
                        DELTARXBRKEN_BIT  = (1 << DELTARXBRKEN_POS),
                        STARTEN_BIT       = (1 << STARTEN_POS     ),
                        FRAMERREN_BIT     = (1 << FRAMERREN_POS   ),
                        PARITYERREN_BIT   = (1 << PARITYERREN_POS ),
                        RXNOISEEN_BIT     = (1 << RXNOISEEN_POS   ),
                        ABERREN_BIT       = (1 << ABERREN_POS     );
} // namespace intenset

namespace intenclr {
static const uint8_t    RXRDYCLR_POS       =  0,
                        TXRDYCLR_POS       =  2,
                        TXIDLECLR_POS      =  3,
                        DELTACTSCLR_POS    =  5,
                        TXDISCLR_POS       =  6,
                        OVERRUNCLR_POS     =  8,
                        DELTARXBRKCLR_POS  = 11,
                        STARTCLR_POS       = 12,
                        FRAMERRCLR_POS     = 13,
                        PARITYERRCLR_POS   = 14,
                        RXNOISECLR_POS     = 15,
                        ABERRCLR_POS       = 16;
static const uint32_t   RXRDYCLR_BIT       = (1 << RXRDYCLR_POS     ),
                        TXRDYCLR_BIT       = (1 << TXRDYCLR_POS     ),
                        TXIDLECLR_BIT      = (1 << TXIDLECLR_POS    ),
                        DELTACTSCLR_BIT    = (1 << DELTACTSCLR_POS  ),
                        TXDISCLR_BIT       = (1 << TXDISCLR_POS     ),
                        OVERRUNCLR_BIT     = (1 << OVERRUNCLR_POS   ),
                        DELTARXBRKCLR_BIT  = (1 << DELTARXBRKCLR_POS),
                        STARTCLR_BIT       = (1 << STARTCLR_POS     ),
                        FRAMERRCLR_BIT     = (1 << FRAMERRCLR_POS   ),
                        PARITYERRCLR_BIT   = (1 << PARITYERRCLR_POS ),
                        RXNOISECLR_BIT     = (1 << RXNOISECLR_POS   ),
                        ABERRCLR_BIT       = (1 << ABERRCLR_POS     );
} // namespace intenclr

namespace rxdat {
static const uint8_t    RXDAT_POS  = 0;
static const uint32_t   RXDAT_MASK = (0x1ff << RXDAT_POS);
} // namespace rxdat

namespace rxdatstat {
static const uint8_t    RXDAT_POS      =  0,
                        FRAMERR_POS    = 13,
                        PARITYERR_POS  = 14,
                        RXNOISE_POS    = 15;
static const uint32_t   RXDAT_MASK     = (0x1ff << RXDAT_POS    ),
                        FRAMERR_BIT    = (    1 << FRAMERR_POS  ),
                        PARITYERR_BIT  = (    1 << PARITYERR_POS),
                        RXNOISE_BIT    = (    1 << RXNOISE_POS  );
} // rxdatstat

namespace txdat {
static const uint8_t    TXDAT_POS  = 0;
static const uint32_t   TXDAT_MASK = (0x1ff << TXDAT_POS);
} // namespace txdat

namespace brg {
static const uint8_t    BRGVAL_POS  = 0;
static const uint32_t   BRGVAL_MASK = (0xffff << BRGVAL_POS);
} // namespace brg

namespace intstat {
static const uint8_t    RXRDY_POS         =  0,
                        TXRDY_POS         =  2,
                        TXIDLE_POS        =  3,
                        DELTACTS_POS      =  5,
                        TXDISINT_POS      =  6,
                        OVERRUNINT_POS    =  8,
                        DELTARXBRK_POS    = 11,
                        START_POS         = 12,
                        FRAMERRINT_POS    = 13,
                        PARITYERRINT_POS  = 14,
                        RXNOISEINT_POS    = 15,
                        ABERR_POS         = 16;
static const uint32_t   RXRDY_BIT         = (1 << RXRDY_POS       ),
                        TXRDY_BIT         = (1 << TXRDY_POS       ),
                        TXIDLE_BIT        = (1 << TXIDLE_POS      ),
                        DELTACTS_BIT      = (1 << DELTACTS_POS    ),
                        TXDISINT_BIT      = (1 << TXDISINT_POS    ),
                        OVERRUNINT_BIT    = (1 << OVERRUNINT_POS  ),
                        DELTARXBRK_BIT    = (1 << DELTARXBRK_POS  ),
                        START_BIT         = (1 << START_POS       ),
                        FRAMERRINT_BIT    = (1 << FRAMERRINT_POS  ),
                        PARITYERRINT_BIT  = (1 << PARITYERRINT_POS),
                        RXNOISEINT_BIT    = (1 << RXNOISEINT_POS  ),
                        ABERR_BIT         = (1 << ABERR_POS       );
} // namespace intstat

namespace osr {
static const uint8_t    OSRVAL_POS  = 0;
static const uint32_t   OSRVAL_MASK = (0xf << OSRVAL_POS);
} // namespace osr

namespace addr {
static const uint8_t    ADDRESS_POS  = 0;
static const uint32_t   ADDRESS_MASK = (0xff << ADDRESS_POS);
} // namespace addr

} // namespace usart


namespace spi {
namespace cfg {
static const uint8_t    ENABLE_POS = 0,
                        MASTER_POS = 2,
                        LSBF_POS   = 3,
                        CPHA_POS   = 4,
                        CPOL_POS   = 5,
                        LOOP_POS   = 7,
#ifdef LPC81X
                        SPOL_POS   = 8
#endif
#ifdef LPC82X
                        SPOL0_POS  =  8,
                        SPOL1_POS  =  9,
                        SPOL2_POS  = 10,
                        SPOL3_POS  = 11
#endif
                        ;
static const uint32_t   ENABLE_BIT = (1 << ENABLE_POS),
                        MASTER_BIT = (1 << MASTER_POS),
                        LSBF_BIT   = (1 << LSBF_POS),
                        CPHA_BIT   = (1 << CPHA_POS),
                        CPOL_BIT   = (1 << CPOL_POS),
                        LOOP_BIT   = (1 << LOOP_POS),
#ifdef LPC81X
                        SPOL_BIT   = (1 << SPOL_POS)
#endif
#ifdef LPC82X
                        SPOL0_BIT  = (1 << SPOL0_POS),
                        SPOL1_BIT  = (1 << SPOL1_POS),
                        SPOL2_BIT  = (1 << SPOL2_POS),
                        SPOL3_BIT  = (1 << SPOL3_POS)
#endif
                        ;
} // namespace cfg

namespace stat {
    static const uint8_t    RXRDY_POS       = 0,
                            TXRDY_POS       = 1,
                            RXOV_POS        = 2,
                            TXUR_POS        = 3,
                            SSA_POS         = 4,
                            SSD_POS         = 5,
                            STALLED_POS     = 6,
                            ENDTRANSFER_POS = 7,
                            MSTIDLE_POS     = 8;
    static const uint32_t   RXRDY_BIT       = (1 << RXRDY_POS      ),
                            TXRDY_BIT       = (1 << TXRDY_POS      ),
                            RXOV_BIT        = (1 << RXOV_POS       ),
                            TXUR_BIT        = (1 << TXUR_POS       ),
                            SSA_BIT         = (1 << SSA_POS        ),
                            SSD_BIT         = (1 << SSD_POS        ),
                            STALLED_BIT     = (1 << STALLED_POS    ),
                            ENDTRANSFER_BIT = (1 << ENDTRANSFER_POS),
                            MSTIDLE_BIT     = (1 << MSTIDLE_POS    );
} // stat

namespace inten_set_clr {
static const uint8_t    RXRDYEN_POS = 0,
                        TXRDYEN_POS = 1,
                        RXOVEN_POS  = 2,
                        TXUREN_POS  = 3,
                        SSAEN_POS   = 4,
                        SSDEN_POS   = 5;
static const uint32_t   RXRDYEN_BIT = (1 << RXRDYEN_POS),
                        TXRDYEN_BIT = (1 << TXRDYEN_POS),
                        RXOVEN_BIT  = (1 << RXOVEN_POS ),
                        TXUREN_BIT  = (1 << TXUREN_POS ),
                        SSAEN_BIT   = (1 << SSAEN_POS  ),
                        SSDEN_BIT   = (1 << SSDEN_POS  );
} // namespace inten_set_clr

namespace txdatctl {
static const uint8_t    TXDAT_POS     =  0,
                        TXSSEL0_N_POS = 16,
                        TXSSEL1_N_POS = 17,
                        TXSSEL2_N_POS = 18,
                        TXSSEL3_N_POS = 19,
                        EOT_POS       = 20,
                        EOF_POS       = 21,
                        RXIGNORE_POS  = 22,
                        LEN_POS       = 24;
static const uint32_t   TXDAT_MASK    =    0xffff            ,
                        TXSSEL0_N_BIT = (  1  << TXSSEL0_N_POS),
                        TXSSEL1_N_BIT = (  1  << TXSSEL1_N_POS),
                        TXSSEL2_N_BIT = (  1  << TXSSEL2_N_POS),
                        TXSSEL3_N_BIT = (  1  << TXSSEL3_N_POS),
                        EOT_BIT       = (  1  << EOT_POS      ),
                        EOF_BIT       = (  1  << EOF_POS      ),
                        RXIGNORE_BIT  = (  1  << RXIGNORE_POS ),
                        LEN_MSK       = (0xf << LEN_POS       );
static const uint32_t   LEN_1_BITS    = (  0  << LEN_POS      ),
                        LEN_2_BITS    = (  1  << LEN_POS      ),
                        LEN_3_BITS    = (  2  << LEN_POS      ),
                        LEN_4_BITS    = (  3  << LEN_POS      ),
                        LEN_5_BITS    = (  4  << LEN_POS      ),
                        LEN_6_BITS    = (  5  << LEN_POS      ),
                        LEN_7_BITS    = (  6  << LEN_POS      ),
                        LEN_8_BITS    = (  7  << LEN_POS      ),
                        LEN_9_BITS    = (  8  << LEN_POS      ),
                        LEN_10_BITS   = (  9  << LEN_POS      ),
                        LEN_11_BITS   = ( 10  << LEN_POS      ),
                        LEN_12_BITS   = ( 11  << LEN_POS      ),
                        LEN_13_BITS   = ( 12  << LEN_POS      ),
                        LEN_14_BITS   = ( 13  << LEN_POS      ),
                        LEN_15_BITS   = ( 14  << LEN_POS      ),
                        LEN_16_BITS   = ( 15  << LEN_POS      );
} // namespace txdatctl

namespace txctrl {
static const uint8_t    TXSSEL0_N_POS = 16,
                        TXSSEL1_N_POS = 17,
                        TXSSEL2_N_POS = 18,
                        TXSSEL3_N_POS = 19,
                        EOT_POS       = 20,
                        EOF_POS       = 21,
                        RXIGNORE_POS  = 22,
                        LEN_POS       = 24;
static const uint32_t   TXSSEL0_N_BIT = (  1 << TXSSEL0_N_POS),
                        TXSSEL1_N_BIT = (  1 << TXSSEL1_N_POS),
                        TXSSEL2_N_BIT = (  1 << TXSSEL2_N_POS),
                        TXSSEL3_N_BIT = (  1 << TXSSEL3_N_POS),
                        EOT_BIT       = (  1 << EOT_POS      ),
                        EOF_BIT       = (  1 << EOF_POS      ),
                        RXIGNORE_BIT  = (  1 << RXIGNORE_POS ),
                        LEN_MASK      = (0xf<< LEN_POS       );
static const uint32_t   LEN_1_BITS    = (  0 << LEN_POS      ),
                        LEN_2_BITS    = (  1 << LEN_POS      ),
                        LEN_3_BITS    = (  2 << LEN_POS      ),
                        LEN_4_BITS    = (  3 << LEN_POS      ),
                        LEN_5_BITS    = (  4 << LEN_POS      ),
                        LEN_6_BITS    = (  5 << LEN_POS      ),
                        LEN_7_BITS    = (  6 << LEN_POS      ),
                        LEN_8_BITS    = (  7 << LEN_POS      ),
                        LEN_9_BITS    = (  8 << LEN_POS      ),
                        LEN_10_BITS   = (  9 << LEN_POS      ),
                        LEN_11_BITS   = ( 10 << LEN_POS      ),
                        LEN_12_BITS   = ( 11 << LEN_POS      ),
                        LEN_13_BITS   = ( 12 << LEN_POS      ),
                        LEN_14_BITS   = ( 13 << LEN_POS      ),
                        LEN_15_BITS   = ( 14 << LEN_POS      ),
                        LEN_16_BITS   = ( 15 << LEN_POS      );
} // namespace txctl

namespace rxdat {
static const uint8_t    RXSSEL0_N_POS = 16,
                        RXSSEL1_N_POS = 17,
                        RXSSEL2_N_POS = 18,
                        RXSSEL3_N_POS = 19,
                        SOT_POS       = 20;
static const uint32_t   RXDAT_MASK    = 0xffff,
                        RXSSEL0_N_BIT = (1 << RXSSEL0_N_POS),
                        RXSSEL1_N_BIT = (1 << RXSSEL1_N_POS),
                        RXSSEL2_N_BIT = (1 << RXSSEL2_N_POS),
                        RXSSEL3_N_BIT = (1 << RXSSEL3_N_POS),
                        SOT_BIT       = (1 << SOT_POS      );
} // namespace rxdat
} // namespace spi


namespace i2c {
namespace cfg {
static const uint8_t    MSTEN_POS     = 0,
                        SLVEN_POS     = 1,
                        MONEN_POS     = 2,
                        TIMEOUTEN_POS = 3,
                        MONCLKSTR_POS = 4;
static const uint32_t   MSTEN_BIT     = (1 << MSTEN_POS),
                        SLVEN_BIT     = (1 << SLVEN_POS),
                        MONEN_BIT     = (1 << MONEN_POS),
                        TIMEOUTEN_BIT = (1 << TIMEOUTEN_POS),
                        MONCLKSTR_BIT = (1 << MONCLKSTR_POS);
} // namespace cfg

namespace stat {
static const uint8_t    MSTPENDING_POS      =  0,
                        MSTSTATE_POS        =  1,
                        MSTARBLOSS_POS      =  4,
                        MSTSTSTPERR_POS     =  6,
                        SLVPENDING_POS      =  8,
                        SLVSTATE_POS        =  9,
                        SLVNOTSTR_POS       = 11,
                        SLVIDX_POS          = 12,
                        SLVSEL_POS          = 14,
                        SLVDESEL_POS        = 15,
                        MONRDY_POS          = 16,
                        MONOV_POS           = 17,
                        MONACTIVE_POS       = 18,
                        MONIDLE_POS         = 19,
                        EVENTTIMEOUT_POS    = 24,
                        SCLTIMEOUT          = 25;
static const uint32_t   MSTPENDING_BIT      = (  1 << MSTPENDING_POS  ),
                        MSTSTATE_MASK       = (0x3 << MSTSTATE_POS    ),
                          MSTSTATE_IDLE     = (0x0 << MSTSTATE_POS    ),
                          MSTSTATE_RECVRDY  = (0x1 << MSTSTATE_POS    ),
                          MSTSTATE_XMITRDY  = (0x2 << MSTSTATE_POS    ),
                          MSTSTATE_NACKADDR = (0x3 << MSTSTATE_POS    ),
                          MSTSTATE_NACKDATA = (0x4 << MSTSTATE_POS    ),
                        MSTARBLOSS_BIT      = (  1 << MSTARBLOSS_POS  ),
                        MSTSTSTPERR_BIT     = (  1 << MSTSTSTPERR_POS ),
                        SLVPENDING_BIT      = (  1 << SLVPENDING_POS  ),
                        SLVSTATE_MASK       = (0x3 << SLVSTATE_POS    ),
                          SLVSTATE_ADDR     = (0x0 << SLVSTATE_POS    ),
                          SLVSTATE_RECV     = (0x1 << SLVSTATE_POS    ),
                          SLVSTATE_XMIT     = (0x2 << SLVSTATE_POS    ),
                        SLVNOTSTR_BIT       = (  1 << SLVNOTSTR_POS   ),
                        SLVIDX_MASK         = (0x3 << SLVIDX_POS      ),
                          SLVIDX_0          = (0x0 << SLVIDX_POS      ),
                          SLVIDX_1          = (0x1 << SLVIDX_POS      ),
                          SLVIDX_2          = (0x2 << SLVIDX_POS      ),
                          SLVIDX_3          = (0x3 << SLVIDX_POS      ),
                        SLVSEL_BIT          = (  1 << SLVSEL_POS      ),
                        SLVDESEL_BIT        = (  1 << SLVDESEL_POS    ),
                        MONRDY_BIT          = (  1 << MONRDY_POS      ),
                        MONOV_BIT           = (  1 << MONOV_POS       ),
                        MONACTIVE_BIT       = (  1 << MONACTIVE_POS   ),
                        MONIDLE_BIT         = (  1 << MONIDLE_POS     ),
                        EVENTTIMEOUT_BIT    = (  1 << EVENTTIMEOUT_POS),
                        SCLTIMEBIT          = (  1 << SCLTIMEOUT      );
} // namespace stat

namespace intenset {
static const uint8_t    MSTPENDINGEN_POS   =  0,
                        MSTARBLOSSEN_POS   =  4,
                        MSTSTSTPERREN_POS  =  6,
                        SLVPENDINGEN_POS   =  8,
                        SLVNOTSTREN_POS    = 11,
                        SLVDESELEN_POS     = 15,
                        MONRDYEN_POS       = 16,
                        MONOVEN_POS        = 17,
                        MONIDLEEN_POS      = 19,
                        EVENTTIMEOUTEN_POS = 24,
                        SCLTIMEOUTEN_POS   = 25;
static const uint32_t   MSTPENDING_BIT     = (1 << MSTPENDINGEN_POS  ),
                        MSTARBLOSS_BIT     = (1 << MSTARBLOSSEN_POS  ),
                        MSTSTSTPERR_BIT    = (1 << MSTSTSTPERREN_POS ),
                        SLVPENDING_BIT     = (1 << SLVPENDINGEN_POS  ),
                        SLVNOTSTR_BIT      = (1 << SLVNOTSTREN_POS   ),
                        SLVDESEL_BIT       = (1 << SLVDESELEN_POS    ),
                        MONRD_BIT          = (1 << MONRDYEN_POS      ),
                        MONO_BIT           = (1 << MONOVEN_POS       ),
                        MONIDL_BIT         = (1 << MONIDLEEN_POS     ),
                        EVENTTIMEOU_BIT    = (1 << EVENTTIMEOUTEN_POS),
                        SCLTIMEOU_BIT      = (1 << SCLTIMEOUTEN_POS  );
} // namespace intenset


namespace intenclr {
static const uint8_t    MSTPENDINGCLR_POS   =  0,
                        MSTARBLOSSCLR_POS   =  4,
                        MSTSTSTPERRCLR_POS  =  6,
                        SLVPENDINGCLR_POS   =  8,
                        SLVNOTSTRCLR_POS    = 11,
                        SLVDESELCLR_POS     = 15,
                        MONRDYCLR_POS       = 16,
                        MONOVCLR_POS        = 17,
                        MONIDLECLR_POS      = 19,
                        EVENTTIMEOUTCLR_POS = 24,
                        SCLTIMEOUTCLR_POS   = 25;
static const uint32_t   MSTPENDINGCLR_BIT   = (1 << MSTPENDINGCLR_POS  ),
                        MSTARBLOSSCLR_BIT   = (1 << MSTARBLOSSCLR_POS  ),
                        MSTSTSTPERRCLR_BIT  = (1 << MSTSTSTPERRCLR_POS ),
                        SLVPENDINGCLR_BIT   = (1 << SLVPENDINGCLR_POS  ),
                        SLVNOTSTRCLR_BIT    = (1 << SLVNOTSTRCLR_POS   ),
                        SLVDESELCLR_BIT     = (1 << SLVDESELCLR_POS    ),
                        MONRDYCLR_BIT       = (1 << MONRDYCLR_POS      ),
                        MONOVCLR_BIT        = (1 << MONOVCLR_POS       ),
                        MONIDLECLR_BIT      = (1 << MONIDLECLR_POS     ),
                        EVENTTIMEOUTCLR_BIT = (1 << EVENTTIMEOUTCLR_POS),
                        SCLTIMEOUTCLR_BIT   = (1 << SCLTIMEOUTCLR_POS  );
} // namespace intenclr

namespace timeout {
static const uint8_t    TOMIN      = 0,
                        TO         = 4;
static const uint32_t   TOMIN_BITS = (0x00f << TOMIN),
                        TO_BITS    = (0xfff << TO   );

} // namespace timeout

namespace clkdiv {
static const uint8_t    DIVVAL_POS  = 0;
static const uint32_t   DIVVAL_BITS = (0xffff << DIVVAL_POS);
} // namespace clkdiv

namespace intstat {
static const uint8_t    MSTPENDING_POS   =  0,
                        MSTARBLOSS_POS   =  4,
                        MSTSTSTPERR_POS  =  6,
                        SLVPENDING_POS   =  8,
                        SLVNOTSTR_POS    = 11,
                        SLVDESEL_POS     = 15,
                        MONRDY_POS       = 16,
                        MONOV_POS        = 17,
                        MONIDLE_POS      = 19,
                        EVENTTIMEOUT_POS = 24,
                        SCLTIMEOUT_POS   = 25;
static const uint32_t   MSTPENDING_BIT   = (1 << MSTPENDING_POS  ),
                        MSTARBLOSS_BIT   = (1 << MSTARBLOSS_POS  ),
                        MSTSTSTPER_BIT   = (1 << MSTSTSTPERR_POS ),
                        SLVPENDING_BIT   = (1 << SLVPENDING_POS  ),
                        SLVNOTST_BIT     = (1 << SLVNOTSTR_POS   ),
                        SLVDESEL_BIT     = (1 << SLVDESEL_POS    ),
                        MONRD_BIT        = (1 << MONRDY_POS      ),
                        MONO_BIT         = (1 << MONOV_POS       ),
                        MONIDLE_BIT      = (1 << MONIDLE_POS     ),
                        EVENTTIMEOUT_BIT = (1 << EVENTTIMEOUT_POS),
                        SCLTIMEOUT_BIT   = (1 << SCLTIMEOUT_POS  );
} // namespace intenstat


namespace mstctl {
static const uint8_t    MSTCONTINUE_POS = 0,
                        MSTSTART_POS    = 1,
                        MSTSTOP_POS     = 2;
static const uint32_t   MSTCONTINUE_BIT = (1 << MSTCONTINUE_POS),
                        MSTSTART_BIT    = (1 << MSTSTART_POS   ),
                        MSTSTOP_BIT     = (1 << MSTSTOP_POS    );
} // namespace mstctl

namespace msttime {
static const uint8_t    MSTSCLLOW_POS   = 0,
                        MSTSCLHIGH_POS  = 4;
static const uint32_t   MSTSCLLOW_MASK  = (0x7 << MSTSCLLOW_POS ),
                          MSTSCLLOW_2   = (0x0 << MSTSCLLOW_POS ),
                          MSTSCLLOW_3   = (0x1 << MSTSCLLOW_POS ),
                          MSTSCLLOW_4   = (0x2 << MSTSCLLOW_POS ),
                          MSTSCLLOW_5   = (0x3 << MSTSCLLOW_POS ),
                          MSTSCLLOW_6   = (0x4 << MSTSCLLOW_POS ),
                          MSTSCLLOW_7   = (0x5 << MSTSCLLOW_POS ),
                          MSTSCLLOW_8   = (0x6 << MSTSCLLOW_POS ),
                          MSTSCLLOW_9   = (0x7 << MSTSCLLOW_POS ),
                        MSTSCLHIGH_MASK = (0x7 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_2  = (0x0 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_3  = (0x1 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_4  = (0x2 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_5  = (0x3 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_6  = (0x4 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_7  = (0x5 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_8  = (0x6 << MSTSCLHIGH_POS),
                          MSTSCLHIGH_9  = (0x7 << MSTSCLHIGH_POS);
} // namespace msttime

namespace mstdat {
static const uint8_t    DATA_POS  = 0,
                        ADDR_POS  = 1;  // ?? assuming is full 8 bits with R/W
static const uint32_t   DATA_BITS = (0xff << DATA_POS );
} // namespace mstdat

namespace slvctl {
static const uint8_t    SLVCONTINUE_POS = 0,
                        SLVNACK_POS     = 1;
static const uint32_t   SLVCONTINUE_BIT = (1 << SLVCONTINUE_POS),
                        SLVNACK_BIT     = (1 << SLVNACK_POS    );
} // namespace slvctl

namespace slvdat {
static const uint8_t    DATA_POS  = 0,
                        ADDR_POS  = 1;  // ?? assuming is full 8 bits with R/W
static const uint32_t   DATA_MASK = (0xff << DATA_POS);
} // namespace slvdat

namespace slvadr {
static const uint8_t    SADISABLE_POS  = 0,
                        SLVADR_POS     = 1;
static const uint32_t   SADISABLE_BIT  = (   1 << SADISABLE_POS),
                        SLVADR_MASK    = (0x7f << SLVADR_POS   );
} // namespace slvadr

namespace slvqual0 {
static const uint8_t    QUALMODE0_POS = 0,
                        SLVQUAL0_POS  = 1;
static const uint32_t   QUALMODE0_BIT = (   1 << QUALMODE0_POS),
                        SLVQUAL0_MASK = (0x7f << SLVQUAL0_POS );
} // namespace slvqual0

namespace monrxdat {
static const uint8_t    MONRXDAT_POS   =  0,
                        MONSTART_POS   =  8,
                        MONRESTART_POS =  9,
                        MONNACK_POS    = 10;
static const uint32_t   MONRXDAT_MASK  = (0x7f << MONRXDAT_POS  ),
                        MONSTART_BIT   = (   1 << MONSTART_POS  ),
                        MONRESTART_BIT = (   1 << MONRESTART_POS),
                        MONNACK_BIT    = (   1 << MONNACK_POS   );
} // namespace monrxdat
} // namespace i2c


namespace sct {
namespace config {
static const uint8_t    UNIFY_POS              =  0,
                        CLKMODE_POS            =  1,
                        CLKSEL_POS             =  3,
                        NORELOAD_U_POS         =  7,
                        NORELOAD_L_POS         =  7,
                        NORELOAD_H_POS         =  8,
                        INSYNC_POS             =  9,
                        AUTOLIMIT_U_POS        = 17,
                        AUTOLIMIT_L_POS        = 17,
                        AUTOLIMIT_H_POS        = 18;
static const uint32_t   UNIFY_BIT              = (1 << UNIFY_POS         ),
                        CLKMODE_MASK           = (0x3 << CLKMODE_POS     ),
                          CLKMODE_SYSTEM_BITS  = (0x0 << CLKMODE_POS     ),
                          CLKMODE_SAMPLED_BITS = (0x1 << CLKMODE_POS     ),
                          CLKMODE_INPUT_BITS   = (0x2 << CLKMODE_POS     ),
                          CLKMODE_ASYNC_BITS   = (0x3 << CLKMODE_POS     ),
                        CLKSEL_MASK            = (0x7 << CLKSEL_POS      ),
                          CLKSEL_RISE_0_BITS   = (0x0 << CLKSEL_POS      ),
                          CLKSEL_FALL_0_BITS   = (0x1 << CLKSEL_POS      ),
                          CLKSEL_RISE_1_BITS   = (0x2 << CLKSEL_POS      ),
                          CLKSEL_FALL_1_BITS   = (0x3 << CLKSEL_POS      ),
                          CLKSEL_RISE_2_BITS   = (0x4 << CLKSEL_POS      ),
                          CLKSEL_FALL_2_BITS   = (0x5 << CLKSEL_POS      ),
                          CLKSEL_RISE_3_BITS   = (0x6 << CLKSEL_POS      ),
                          CLKSEL_FALL_3_BITS   = (0x7 << CLKSEL_POS      ),
                        NORELOAD_U_BIT         = (  1 << NORELOAD_U_POS  ),
                        NORELOAD_L_BIT         = (  1 << NORELOAD_L_POS  ),
                        NORELOAD_H_BIT         = (  1 << NORELOAD_H_POS  ),
                        INSYNC_MASK            = (0xf << INSYNC_POS      ),
                          INSYNC_INPUT_0_BIT   = (0x1 << INSYNC_POS      ),
                          INSYNC_INPUT_1_BIT   = (0x2 << INSYNC_POS      ),
                          INSYNC_INPUT_2_BIT   = (0x4 << INSYNC_POS      ),
                          INSYNC_INPUT_3_BIT   = (0x8 << INSYNC_POS      ),
                        AUTOLIMIT_U_BIT        = (   1 << AUTOLIMIT_U_POS),
                        AUTOLIMIT_L_BIT        = (   1 << AUTOLIMIT_L_POS),
                        AUTOLIMIT_H_BIT        = (   1 << AUTOLIMIT_H_POS);
} // namspace config

namespace ctrl {
static const uint8_t    DOWN_L_POS      = 0,
                        DOWN_U_POS      = 0,
                        STOP_L_POS      = 1,
                        STOP_U_POS      = 1,
                        HALT_L_POS      = 2,
                        HALT_U_POS      = 2,
                        CLRCTR_L_POS    = 3,
                        CLRCTR_U_POS    = 3,
                        BIDIR_L_POS     = 4,
                        BIDIR_U_POS     = 4,
                        PRESCALE_L_POS  = 5,
                        PRESCALE_U_POS  = 5,
                        DOWN_H_POS      = 16,
                        STOP_H_POS      = 17,
                        HALT_H_POS      = 18,
                        CLRCTR_H_POS    = 19,
                        BIDIR_H_POS     = 20,
                        PRESCALE_H_POS  = 21;
static const uint32_t   DOWN_L_BIT      = (   1 << DOWN_L_POS    ),
                        DOWN_U_BIT      = (   1 << DOWN_U_POS    ),
                        STOP_L_BIT      = (   1 << STOP_L_POS    ),
                        STOP_U_BIT      = (   1 << STOP_U_POS    ),
                        HALT_L_BIT      = (   1 << HALT_L_POS    ),
                        HALT_U_BIT      = (   1 << HALT_U_POS    ),
                        CLRCTR_L_BIT    = (   1 << CLRCTR_L_POS  ),
                        CLRCTR_U_BIT    = (   1 << CLRCTR_U_POS  ),
                        BIDIR_L_BIT     = (   1 << BIDIR_L_POS   ),
                        BIDIR_U_BIT     = (   1 << BIDIR_U_POS   ),
                        PRESCALE_L_MASK = (0xff << PRESCALE_L_POS),
                        PRESCALE_U_MASK = (0xff << PRESCALE_U_POS),
                        DOWN_H_BIT      = (   1 << DOWN_H_POS    ),
                        STOP_H_BIT      = (   1 << STOP_H_POS    ),
                        HALT_H_BIT      = (   1 << HALT_H_POS    ),
                        CLRCTR_H_BIT    = (   1 << CLRCTR_H_POS  ),
                        BIDIR_H_BIT     = (   1 << BIDIR_H_POS   ),
                        PRESCALE_H_MASK = (0xff << PRESCALE_H_POS);
} // namspace ctrl

namespace evflag {
static const uint8_t    EVENT_0_POS = 0,
                        EVENT_1_POS = 1,
                        EVENT_2_POS = 2,
                        EVENT_3_POS = 3,
                        EVENT_4_POS = 4,
                        EVENT_5_POS = 5,
                        EVENT_6_POS = 6,
                        EVENT_7_POS = 7;
static const uint32_t   EVENT_0_BIT = (1 << EVENT_0_POS),
                        EVENT_1_BIT = (1 << EVENT_1_POS),
                        EVENT_2_BIT = (1 << EVENT_2_POS),
                        EVENT_3_BIT = (1 << EVENT_3_POS),
                        EVENT_4_BIT = (1 << EVENT_4_POS),
                        EVENT_5_BIT = (1 << EVENT_5_POS),
                        EVENT_6_BIT = (1 << EVENT_6_POS),
                        EVENT_7_BIT = (1 << EVENT_7_POS);
} // namspace evflag

namespace event {
namespace state {
static const uint8_t    STATEMSK_0_POS   = 0,
                        STATEMSK_1_POS   = 1,
                        STATEMSK_2_POS   = 2,
                        STATEMSK_3_POS   = 3,
                        STATEMSK_4_POS   = 4,
                        STATEMSK_5_POS   = 5,
                        STATEMSK_6_POS   = 6,
                        STATEMSK_7_POS   = 7;
static const uint32_t   STATEMASK_MASK   = (0xff << STATEMSK_0_POS),
                          STATEMSK_0_BIT = (1    << STATEMSK_0_POS),
                          STATEMSK_1_BIT = (1    << STATEMSK_1_POS),
                          STATEMSK_2_BIT = (1    << STATEMSK_2_POS),
                          STATEMSK_3_BIT = (1    << STATEMSK_3_POS),
                          STATEMSK_4_BIT = (1    << STATEMSK_4_POS),
                          STATEMSK_5_BIT = (1    << STATEMSK_5_POS),
                          STATEMSK_6_BIT = (1    << STATEMSK_6_POS),
                          STATEMSK_7_BIT = (1    << STATEMSK_7_POS);
} // namespace state
namespace ctrl {
static const uint8_t    MATCHSEL_POS          =  0,
                        HEVENT_POS            =  4,
                        OUTSEL_POS            =  5,
                        IOSEL_POS             =  6,
                        IOCOND_POS            = 10,
                        COMBMODE_POS          = 12,
                        STATELD_POS           = 14,
                        STATEV_POS            = 15,
                        MATCHMEM_POS          = 20,
                        DIRECTION_POS         = 21;
static const uint32_t   MATCHSEL_MASK         = (0xf  << MATCHSEL_POS ),
                        HEVENT_BIT            = (1    << HEVENT_POS   ),
                        OUTSEL_BIT            = (1    << OUTSEL_POS   ),
                        IOSEL_BIT             = (1    << IOSEL_POS    ),
                        IOCOND_MASK           = (0x3  << IOCOND_POS   ),
                          IOCOND_LOW_BITS     = (0x0  << IOCOND_POS   ),
                          IOCOND_RISE_BITS    = (0x1  << IOCOND_POS   ),
                          IOCOND_FALL_BITS    = (0x2  << IOCOND_POS   ),
                          IOCOND_HIGH_BITS    = (0x3  << IOCOND_POS   ),
                        COMBMODE_MASK         = (0x3  << COMBMODE_POS ),
                          COMBMODE_OR_BITS    = (0x0  << COMBMODE_POS ),
                          COMBMODE_MATCH_BITS = (0x1  << COMBMODE_POS ),
                          COMBMODE_IO_BITS    = (0x2  << COMBMODE_POS ),
                          COMBMODE_AND_BITS   = (0x3  << COMBMODE_POS ),
                        STATELD_MASK          = (0x1  << STATELD_POS  ),
                          STATELD_ADD_BIT     = (0x0  << STATELD_POS  ),
                          STATELD_LOAD_BIT    = (0x1  << STATELD_POS  ),
                        STATEV_MASK           = (0x1f << STATEV_POS   ),
                        MATCHMEM_BIT          = (1    << MATCHMEM_POS ),
                        DIRECTION_MASK        = (0x3  << DIRECTION_POS),
                          DIRECTION_BOTH_BITS = (0x0  << DIRECTION_POS),
                          DIRECTION_UP_BITS   = (0x1  << DIRECTION_POS),
                          DIRECTION_DOWN_BITS = (0x2  << DIRECTION_POS);
} // namespace ctrl
} // namespace event
} // namespace sct


namespace mrt {
static const uint8_t    CHANNEL_0 = 0,
                        CHANNEL_1 = 1,
                        CHANNEL_2 = 2,
                        CHANNEL_3 = 3;

namespace interval_value {
static const uint8_t        FORCE_LOAD_POS = 31;
static const uint32_t       FORCE_LOAD_BIT = (1 << FORCE_LOAD_POS);
} // namespace interval_value

namespace ctrl {
static const uint8_t    INTERRUPT_ENABLE_POS      = 0,
                        MODE_POS                  = 1;
static const uint32_t   INTERRUPT_ENABLE_BIT      = (1 << INTERRUPT_ENABLE_POS),
                        MODE_MASK                 = (0x3 << MODE_POS          ),
                          REPEAT_INTERRUPT_BITS   = (0x0 << MODE_POS          ),
                          ONE_SHOT_INTERRUPT_BITS = (0x1 << MODE_POS          ),
                          ONE_SHOT_BUS_STALL_BITS = (0x2 << MODE_POS          );
} // namespace ctrl

namespace status {
static const uint8_t    INTERRUPT_PENDING_POS   = 0,
                        IS_RUNNING_POS          = 1;
static const uint32_t   INTERRUPT_PENDING_BIT   = (1 << INTERRUPT_PENDING_POS),
                        IS_RUNNING_BIT          = (1 << IS_RUNNING_POS       );

} // namespace status

namespace idle_ch {
static const uint8_t    CHAN_POS     = 4,
                        NUM_CHANNELS = 4;
static const uint32_t   CHAN_MASK    = (0xf << CHAN_POS);
} // namespace idle_ch
} // namespace mrt


namespace cmp {
namespace ctrl {
static const uint8_t    EDGESEL_POS                 =  3,
                        COMPSA_POS                  =  6,
                        COMP_VP_SEL_POS             =  8,
                        COMP_VM_SEL_POS             = 11,
                        EDGECLR_POS                 = 20,
                        COMPSTAT_POS                = 21,
                        COMPEDGE_POS                = 23,
                        HYS_POS                     = 25;
static const uint32_t   EDGESEL_MASK                = (0x3 << EDGESEL_POS   ),
                          EDGESEL_FALLING_BITS      = (0x0 << EDGESEL_POS   ),
                          EDGESEL_RISING_BITS       = (0x1 << EDGESEL_POS   ),
                          EDGESEL_BOTH_BITS         = (0x2 << EDGESEL_POS   ),
                          EDGESEL_BOTH_BITS_2       = (0x3 << EDGESEL_POS   ),
                        COMPSA_BIT                  = ( 1 << COMPSA_POS     ),
                        COMP_VP_SEL_MASK            = (0x7 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_LADDER_BITS   = (0x0 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_ACMP_I1_BITS  = (0x1 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_ACMP_I2_BITS  = (0x2 << COMP_VP_SEL_POS),
#ifdef LPC82X
                          COMP_VP_SEL_ACMP_I3_BITS  = (0x3 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_ACMP_I4_BITS  = (0x4 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_BANDGAP_BITS  = (0x5 << COMP_VP_SEL_POS),
                          COMP_VP_SEL_ADC0_BITS     = (0x6 << COMP_VP_SEL_POS),
#endif
#ifdef LPC81X
                          COMP_VP_SEL_BANDGAP_BITS  = (0x6 << COMP_VP_SEL_POS),
#endif
                          COMP_VP_SEL_RESERVED_BITS = (0x7 << COMP_VP_SEL_POS),
                        COMP_VM_SEL_MASK            = (0x7 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_LADDER_BITS   = (0x0 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_ACMP_I1_BITS  = (0x1 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_ACMP_I2_BITS  = (0x2 << COMP_VM_SEL_POS),
#ifdef LPC82X
                          COMP_VM_SEL_ACMP_I3_BITS  = (0x3 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_ACMP_I4_BITS  = (0x4 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_BANDGAP_BITS  = (0x5 << COMP_VM_SEL_POS),
                          COMP_VM_SEL_ADC0_BITS     = (0x6 << COMP_VM_SEL_POS),
#endif
#ifdef LPC81X
                          COMP_VM_SEL_BANDGAP_BITS  = (0x6 << COMP_VM_SEL_POS),
#endif
                          COMP_VM_SEL_RESERVED_BITS = (0x7 << COMP_VP_SEL_POS),
                        EDGECLR_BIT                 = ( 1 << EDGECLR_POS    ),
                        COMPSTAT_BIT                = ( 1 << COMPSTAT_POS   ),
                        COMPEDGE_BIT                = ( 1 << COMPEDGE_POS   ),
                        HYS_MASK                    = (0x3 << HYS_POS       ),
                          HYS_NONE_BITS             = (0x0 << HYS_POS       ),
                          HYS_5_MV_BITS             = (0x1 << HYS_POS       ),
                          HYS_10_MV_BITS            = (0x2 << HYS_POS       ),
                          HYS_20_MV_BITS            = (0x3 << HYS_POS       );
} // namespace ctrl

namespace ladder {
static const uint8_t        LADEN_POS                = 0,
                            LADSEL_POS               = 1,
                            LADREF_POS               = 6;
static const uint32_t       LADEN_BIT                = (   1 << LADEN_POS ),
                            LADSEL_MASK              = (0x1f << LADSEL_POS),
                              LADSEL_VSS_BITS        = (0x00 << LADSEL_POS),
                              LADSEL_1_VREF_31_BITS  = (0x01 << LADSEL_POS),
                              LADSEL_2_VREF_31_BITS  = (0x02 << LADSEL_POS),
                              LADSEL_3_VREF_31_BITS  = (0x03 << LADSEL_POS),
                              LADSEL_4_VREF_31_BITS  = (0x04 << LADSEL_POS),
                              LADSEL_5_VREF_31_BITS  = (0x05 << LADSEL_POS),
                              LADSEL_6_VREF_31_BITS  = (0x06 << LADSEL_POS),
                              LADSEL_7_VREF_31_BITS  = (0x07 << LADSEL_POS),
                              LADSEL_8_VREF_31_BITS  = (0x08 << LADSEL_POS),
                              LADSEL_9_VREF_31_BITS  = (0x09 << LADSEL_POS),
                              LADSEL_10_VREF_31_BITS = (0x0a << LADSEL_POS),
                              LADSEL_11_VREF_31_BITS = (0x0b << LADSEL_POS),
                              LADSEL_12_VREF_31_BITS = (0x0c << LADSEL_POS),
                              LADSEL_13_VREF_31_BITS = (0x0d << LADSEL_POS),
                              LADSEL_14_VREF_31_BITS = (0x0e << LADSEL_POS),
                              LADSEL_15_VREF_31_BITS = (0x0f << LADSEL_POS),
                              LADSEL_16_VREF_31_BITS = (0x10 << LADSEL_POS),
                              LADSEL_17_VREF_31_BITS = (0x11 << LADSEL_POS),
                              LADSEL_18_VREF_31_BITS = (0x12 << LADSEL_POS),
                              LADSEL_19_VREF_31_BITS = (0x13 << LADSEL_POS),
                              LADSEL_20_VREF_31_BITS = (0x14 << LADSEL_POS),
                              LADSEL_21_VREF_31_BITS = (0x15 << LADSEL_POS),
                              LADSEL_22_VREF_31_BITS = (0x16 << LADSEL_POS),
                              LADSEL_23_VREF_31_BITS = (0x17 << LADSEL_POS),
                              LADSEL_24_VREF_31_BITS = (0x18 << LADSEL_POS),
                              LADSEL_25_VREF_31_BITS = (0x19 << LADSEL_POS),
                              LADSEL_26_VREF_31_BITS = (0x1a << LADSEL_POS),
                              LADSEL_27_VREF_31_BITS = (0x1b << LADSEL_POS),
                              LADSEL_28_VREF_31_BITS = (0x1c << LADSEL_POS),
                              LADSEL_29_VREF_31_BITS = (0x1d << LADSEL_POS),
                              LADSEL_30_VREF_31_BITS = (0x1e << LADSEL_POS),
                              LADSEL_VREF_BITS       = (0x1f << LADSEL_POS),
                            LADREF_BIT               = (   1 << LADREF_POS),
                              LADREF_VDD_BIT         = (   0 << LADREF_POS),
                              LADREF_VDDCMP_BIT      = (   1 << LADREF_POS);
} // namespace ladder
} // namespace cmp

} // namespace lpc8xx

} // namespace baresil

#endif   /* ifndef LPC8XX_HXX */
