#ifndef STM32F10_12357_XX_HXX
#define STM32F10_12357_XX_HXX

#include <stdint.h>

#include <stm32f103xb.h>


namespace baresil {

namespace stm32f10_12357_xx {

namespace rcc {
namespace cfgr {
static const uint32_t   HPRE_NO_DIV  = 0b0000 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_1   = 0b0000 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_2   = 0b1000 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_4   = 0b1001 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_8   = 0b1010 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_16  = 0b1011 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_64  = 0b1100 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_128 = 0b1101 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_256 = 0b1110 << RCC_CFGR_HPRE_Pos,
                        HPRE_DIV_512 = 0b1111 << RCC_CFGR_HPRE_Pos;
} // namespace cfgr
} // namespace rcc


namespace gpio {
namespace crl {
static const uint8_t    MODE_POS                   =  0,
                        CONF_POS                   =  2,
                        PORT_0_POS                 =  0,
                        PORT_1_POS                 =  4,
                        PORT_2_POS                 =  8,
                        PORT_3_POS                 = 12,
                        PORT_4_POS                 = 16,
                        PORT_5_POS                 = 20,
                        PORT_6_POS                 = 24,
                        PORT_7_POS                 = 28;
static const uint32_t   MODE_MASK                  = (0x3 << MODE_POS),
                          MODE_INPUT               = (0x0 << MODE_POS),
                          MODE_OUTPUT_10MHZ        = (0x1 << MODE_POS),
                          MODE_OUTPUT_2MHZ         = (0x2 << MODE_POS),
                          MODE_OUTPUT_50MHZ        = (0x3 << MODE_POS),
                        CONF_MASK                  = (0x3 << CONF_POS),
                          CONF_INPUT_ANALOG        = (0x0 << CONF_POS),
                          CONF_INPUT_FLOATING      = (0x1 << CONF_POS),
                          CONF_INPUT_PULL_UPDN     = (0x2 << CONF_POS),
                          CONF_INPUT_RESERVED      = (0x3 << CONF_POS),
                          CONF_OUTPUT_PUSH_PULL    = (0x0 << CONF_POS),
                          CONF_OUTPUT_OPEN_DRAIN   = (0x1 << CONF_POS),
                          CONF_ALT_FUNC_PUSH_PULL  = (0x2 << CONF_POS),
                          CONF_ALT_FUNC_OPEN_DRAIN = (0x3 << CONF_POS);
} // namespace crl
namespace crh {
static const uint8_t    MODE_POS                   =  0,
                        CONF_POS                   =  2,
                        PORT_8_POS                 =  0,
                        PORT_9_POS                 =  4,
                        PORT_10_POS                =  8,
                        PORT_11_POS                = 12,
                        PORT_12_POS                = 16,
                        PORT_13_POS                = 20,
                        PORT_14_POS                = 24,
                        PORT_15_POS                = 28;
static const uint32_t   MODE_MASK                  = (0x3 << MODE_POS),
                          MODE_INPUT               = (0x0 << MODE_POS),
                          MODE_OUTPUT_10MHZ        = (0x1 << MODE_POS),
                          MODE_OUTPUT_2MHZ         = (0x2 << MODE_POS),
                          MODE_OUTPUT_50MHZ        = (0x3 << MODE_POS),
                        CONF_MASK                  = (0x3 << CONF_POS),
                          CONF_INPUT_ANALOG        = (0x0 << CONF_POS),
                          CONF_INPUT_FLOATING      = (0x1 << CONF_POS),
                          CONF_INPUT_PULL_UPDN     = (0x2 << CONF_POS),
                          CONF_INPUT_RESERVED      = (0x3 << CONF_POS),
                          CONF_OUTPUT_PUSH_PULL    = (0x0 << CONF_POS),
                          CONF_OUTPUT_OPEN_DRAIN   = (0x1 << CONF_POS),
                          CONF_ALT_FUNC_PUSH_PULL  = (0x2 << CONF_POS),
                          CONF_ALT_FUNC_OPEN_DRAIN = (0x3 << CONF_POS);
} // namespace crh
namespace crlh {  // both combined so can be used if which register unknown
static const uint8_t    MODE_POS                   =  0,
                        CONF_POS                   =  2,
                        PORT_0_POS                 =  0,
                        PORT_1_POS                 =  4,
                        PORT_2_POS                 =  8,
                        PORT_3_POS                 = 12,
                        PORT_4_POS                 = 16,
                        PORT_5_POS                 = 20,
                        PORT_6_POS                 = 24,
                        PORT_7_POS                 = 28,
                        PORT_8_POS                 =  0,
                        PORT_9_POS                 =  4,
                        PORT_10_POS                =  8,
                        PORT_11_POS                = 12,
                        PORT_12_POS                = 16,
                        PORT_13_POS                = 20,
                        PORT_14_POS                = 24,
                        PORT_15_POS                = 28;
static const uint32_t   MODE_MASK                  = (0x3 << MODE_POS),
                          MODE_INPUT               = (0x0 << MODE_POS),
                          MODE_OUTPUT_10MHZ        = (0x1 << MODE_POS),
                          MODE_OUTPUT_2MHZ         = (0x2 << MODE_POS),
                          MODE_OUTPUT_50MHZ        = (0x3 << MODE_POS),
                        CONF_MASK                  = (0x3 << CONF_POS),
                          CONF_INPUT_ANALOG        = (0x0 << CONF_POS),
                          CONF_INPUT_FLOATING      = (0x1 << CONF_POS),
                          CONF_INPUT_PULL_UPDN     = (0x2 << CONF_POS),
                          CONF_INPUT_RESERVED      = (0x3 << CONF_POS),
                          CONF_OUTPUT_PUSH_PULL    = (0x0 << CONF_POS),
                          CONF_OUTPUT_OPEN_DRAIN   = (0x1 << CONF_POS),
                          CONF_ALT_FUNC_PUSH_PULL  = (0x2 << CONF_POS),
                          CONF_ALT_FUNC_OPEN_DRAIN = (0x3 << CONF_POS);
} // namespace crlh
namespace idr {
static const uint8_t    PORT_0_POS  =  0,
                        PORT_1_POS  =  1,
                        PORT_2_POS  =  2,
                        PORT_3_POS  =  3,
                        PORT_4_POS  =  4,
                        PORT_5_POS  =  5,
                        PORT_6_POS  =  6,
                        PORT_7_POS  =  7,
                        PORT_8_POS  =  8,
                        PORT_9_POS  =  9,
                        PORT_10_POS = 10,
                        PORT_11_POS = 11,
                        PORT_12_POS = 12,
                        PORT_13_POS = 13,
                        PORT_14_POS = 14,
                        PORT_15_POS = 15;
static const uint32_t   PORT_0_BIT       = (1 << PORT_0_POS ),
                        PORT_1_BIT       = (1 << PORT_1_POS ),
                        PORT_2_BIT       = (1 << PORT_2_POS ),
                        PORT_3_BIT       = (1 << PORT_3_POS ),
                        PORT_4_BIT       = (1 << PORT_4_POS ),
                        PORT_5_BIT       = (1 << PORT_5_POS ),
                        PORT_6_BIT       = (1 << PORT_6_POS ),
                        PORT_7_BIT       = (1 << PORT_7_POS ),
                        PORT_8_BIT       = (1 << PORT_8_POS ),
                        PORT_9_BIT       = (1 << PORT_9_POS ),
                        PORT_10_BIT      = (1 << PORT_10_POS),
                        PORT_11_BIT      = (1 << PORT_11_POS),
                        PORT_12_BIT      = (1 << PORT_12_POS),
                        PORT_13_BIT      = (1 << PORT_13_POS),
                        PORT_14_BIT      = (1 << PORT_14_POS),
                        PORT_15_BIT      = (1 << PORT_15_POS);
} // namespace idr {
namespace odr {
static const uint8_t    PORT_0_POS  =  0,
                        PORT_1_POS  =  1,
                        PORT_2_POS  =  2,
                        PORT_3_POS  =  3,
                        PORT_4_POS  =  4,
                        PORT_5_POS  =  5,
                        PORT_6_POS  =  6,
                        PORT_7_POS  =  7,
                        PORT_8_POS  =  8,
                        PORT_9_POS  =  9,
                        PORT_10_POS = 10,
                        PORT_11_POS = 11,
                        PORT_12_POS = 12,
                        PORT_13_POS = 13,
                        PORT_14_POS = 14,
                        PORT_15_POS = 15;
static const uint32_t   PORT_0_BIT       = (1 << PORT_0_POS ),
                        PORT_1_BIT       = (1 << PORT_1_POS ),
                        PORT_2_BIT       = (1 << PORT_2_POS ),
                        PORT_3_BIT       = (1 << PORT_3_POS ),
                        PORT_4_BIT       = (1 << PORT_4_POS ),
                        PORT_5_BIT       = (1 << PORT_5_POS ),
                        PORT_6_BIT       = (1 << PORT_6_POS ),
                        PORT_7_BIT       = (1 << PORT_7_POS ),
                        PORT_8_BIT       = (1 << PORT_8_POS ),
                        PORT_9_BIT       = (1 << PORT_9_POS ),
                        PORT_10_BIT      = (1 << PORT_10_POS),
                        PORT_11_BIT      = (1 << PORT_11_POS),
                        PORT_12_BIT      = (1 << PORT_12_POS),
                        PORT_13_BIT      = (1 << PORT_13_POS),
                        PORT_14_BIT      = (1 << PORT_14_POS),
                        PORT_15_BIT      = (1 << PORT_15_POS),
                        PORT_0_PULLDOWN  = (0 << PORT_0_POS ),
                        PORT_1_PULLDOWN  = (0 << PORT_1_POS ),
                        PORT_2_PULLDOWN  = (0 << PORT_2_POS ),
                        PORT_3_PULLDOWN  = (0 << PORT_3_POS ),
                        PORT_4_PULLDOWN  = (0 << PORT_4_POS ),
                        PORT_5_PULLDOWN  = (0 << PORT_5_POS ),
                        PORT_6_PULLDOWN  = (0 << PORT_6_POS ),
                        PORT_7_PULLDOWN  = (0 << PORT_7_POS ),
                        PORT_8_PULLDOWN  = (0 << PORT_8_POS ),
                        PORT_9_PULLDOWN  = (0 << PORT_9_POS ),
                        PORT_10_PULLDOWN = (0 << PORT_10_POS),
                        PORT_11_PULLDOWN = (0 << PORT_11_POS),
                        PORT_12_PULLDOWN = (0 << PORT_12_POS),
                        PORT_13_PULLDOWN = (0 << PORT_13_POS),
                        PORT_14_PULLDOWN = (0 << PORT_14_POS),
                        PORT_15_PULLDOWN = (0 << PORT_15_POS),
                        PORT_0_PULLUP    = (1 << PORT_0_POS ),
                        PORT_1_PULLUP    = (1 << PORT_1_POS ),
                        PORT_2_PULLUP    = (1 << PORT_2_POS ),
                        PORT_3_PULLUP    = (1 << PORT_3_POS ),
                        PORT_4_PULLUP    = (1 << PORT_4_POS ),
                        PORT_5_PULLUP    = (1 << PORT_5_POS ),
                        PORT_6_PULLUP    = (1 << PORT_6_POS ),
                        PORT_7_PULLUP    = (1 << PORT_7_POS ),
                        PORT_8_PULLUP    = (1 << PORT_8_POS ),
                        PORT_9_PULLUP    = (1 << PORT_9_POS ),
                        PORT_10_PULLUP   = (1 << PORT_10_POS),
                        PORT_11_PULLUP   = (1 << PORT_11_POS),
                        PORT_12_PULLUP   = (1 << PORT_12_POS),
                        PORT_13_PULLUP   = (1 << PORT_13_POS),
                        PORT_14_PULLUP   = (1 << PORT_14_POS),
                        PORT_15_PULLUP   = (1 << PORT_15_POS);
} // namespace odr {
} // namespace gpio


namespace afio {
                        // indices into AFIO->EXTICR[4] to match ST docs
static const uint8_t    EXTICR1 = 0,
                        EXTICR2 = 1,
                        EXTICR3 = 2,
                        EXTICR4 = 3;
#if 0   // already in stm32f103xb.h in different, explicit-per-register forms
namespace exticr {
static const uint8_t    PA_X_BITS = 0x0,
                        PB_X_BITS = 0x1,
                        PC_X_BITS = 0x2,
                        PD_X_BITS = 0x3,
                        PE_X_BITS = 0x5,
                        PF_X_BITS = 0x5,
                        PG_X_BITS = 0x6;
} // namespace exticr
#endif
} // namespace afio


namespace tim_2_5 {
namespace smcr {
/*
    rm0410_f7_6,7_xxx_reference_manual.pdf
    p.406  15.4.3 TIMx slave mode control register (TIMx_SMCR)
           Bits 6:4 TS: Trigger selection
           Bits 2:0 SMS: Slave mode selection
    p.408  Table 86. TIMx Internal trigger connection (1)
*/
static const uint8_t    TS_POS             = 4               ;
static const uint32_t   TS_MASK            = 0b111 << TS_POS ,
                          TS_ITR0          = 0b000 << TS_POS ,
                            TS_MSTR1_SLAV2 = TS_ITR0         ,
                            TS_MSTR1_SLAV3 = TS_ITR0         ,
                            TS_MSTR1_SLAV4 = TS_ITR0         ,
                            TS_MSTR2_SLAV5 = TS_ITR0         ,
                          TS_ITR1          = 0b001 << TS_POS ,
                            TS_MSTR8_SLAV2 = TS_ITR1         ,
                            TS_MSTR2_SLAV3 = TS_ITR1         ,
                            TS_MSTR2_SLAV4 = TS_ITR1         ,
                            TS_MSTR3_SLAV5 = TS_ITR1         ,
                          TS_ITR2          = 0b010 << TS_POS ,
                            TS_MSTR3_SLAV2 = TS_ITR2         ,
                            TS_MSTR5_SLAV3 = TS_ITR2         ,
                            TS_MSTR3_SLAV4 = TS_ITR2         ,
                            TS_MSTR4_SLAV5 = TS_ITR2         ,
                          TS_ITR3          = 0b011 << TS_POS ,
                            TS_MSTR4_SLAV2 = TS_ITR3         ,
                            TS_MSTR4_SLAV3 = TS_ITR3         ,
                            TS_MSTR8_SLAV4 = TS_ITR3         ,
                            TS_MSTR8_SLAV5 = TS_ITR3         ,
                          TS_ED            = 0b100 << TS_POS ,
                          TS_TI1FP1        = 0b101 << TS_POS ,
                          TS_TI2FP2        = 0b110 << TS_POS ,
                          TS_ETRF          = 0b111 << TS_POS ;

static const uint8_t    SMS_POS            = 0               ;
static const uint32_t   SMS_MASK           = 0b111 << SMS_POS,
                          SMS_DISABLED     = 0b000 << SMS_POS,
                          SMS_ENCODER_1    = 0b001 << SMS_POS,
                          SMS_ENCODER_2    = 0b010 << SMS_POS,
                          SMS_RESET        = 0b100 << SMS_POS,
                          SMS_GATED        = 0b101 << SMS_POS,
                          SMS_TRIGGER      = 0b110 << SMS_POS,
                          SMS_EXTERNAL     = 0b111 << SMS_POS;
} // namespace smcr
} // namespace tim_2_5


namespace i2c {
namespace oar1 {
static const uint8_t        ADD_0_POS       =  0,
                            ADD_7_1_POS     =  1,
                            ADD_9_8_POS     =  8,
                            BIT_14_POS      = 14,
                            ADD_MODE_POS    = 15;
static const uint32_t       ADD_0_MASK      = (0x1  << ADD_0_POS   ),
                            ADD_7_1_MASK    = (0x7f << ADD_7_1_POS ),
                            ADD_9_8_MASK    = (0x3  << ADD_9_8_POS ),
                            BIT_14_BIT      = (  1  << BIT_14_POS  ),
                            ADD_MODE_7_BIT  = (  0  << ADD_MODE_POS),
                            ADD_MODE_10_BIT = (  1  << ADD_MODE_POS);
} // namespace oar1
} // namespace i2c

} // namespace stm32f10_12357_xx
} // namespace baresil

#endif   // ifndef STM32F10_12357_XX_HXX
