#ifndef TRI2B_CONFIG_HXX
#define TRI2B_CONFIG_HXX

#include <stdint.h>

#include <LPC82x.h>

#include <lpc8xx.hxx>


using namespace baresil::lpc8xx;

namespace tri2b_config {

#ifdef TRIQUAD_INTERRUPTS
// must match
static const IRQn_Type      ALRT_IRQN = PININT4_IRQn,
                            LTCH_IRQN = PININT5_IRQn,
                            STRT_IRQN = Reserved11_IRQn;
#define TRI2B_ALRT_ISR  PININT4_IRQHandler
#define TRI2B_LTCH_ISR  PININT5_IRQHandler
#define TRI2B_STRT_ISR  Reserved1_IRQHandler
static const uint32_t       ALRT_NVIC_BIT = nvic::PININT4_BIT,
                            LTCH_NVIC_BIT = nvic::PININT5_BIT,
                            STRT_NVIC_BIT = nvic::RESERVED1_BIT;
#endif

// special open drain ports -- don't try to set open drain in IOCON
#define TRI2B_DATA_PORT_IS_I2C
#define TRI2B_ALRT_PORT_IS_I2C
#undef  TRI2B_LTCH_PORT_IS_I2C

#undef TRI2B_USING_RESETN_AS_GPIO

static volatile uint32_t* const     ALRT_IOCON_PIO_REG = &LPC_IOCON->PIO0_10;
static volatile uint32_t* const     LTCH_IOCON_PIO_REG = &LPC_IOCON-> PIO0_4;
static volatile uint32_t* const     DATA_IOCON_PIO_REG = &LPC_IOCON->PIO0_11;

                        // match
static const uint8_t    ALRT_GPIO_NDX = gpio::PIN_10_POS,   // I2C0 clock
                        LTCH_GPIO_NDX = gpio:: PIN_4_POS,   // ~TRST
                        DATA_GPIO_NDX = gpio::PIN_11_POS;   // I2C0 data
static const uint32_t   ALRT_GPIO_BIT = gpio::PIN_10_BIT,   // I2C0 clock
                        LTCH_GPIO_BIT = gpio:: PIN_4_BIT,   // ~TRST
                        DATA_GPIO_BIT = gpio::PIN_11_BIT;   // I2C0 data


#ifdef TRIQUAD_NXP_GLITCH_FILTER
static volatile uint32_t* const       GLITCH_SYSCON_CLKDIV_REG
                                    = &LPC_SYSCON->IOCONCLKDIV0;     // must match
static const    uint32_t              GLITCH_SYSCON_DIV
                                    = iocon::modes::CLK_DIV_0_BITS;  // must match
#endif


                        // match XXXX_IRQN above
static const uint8_t    ALRT_PINTSEL_NDX = 4,
                        LTCH_PINTSEL_NDX = 5;

static const uint8_t    ALRT_PININT_BIT = 1 << ALRT_PINTSEL_NDX,
                        LTCH_PININT_BIT = 1 << LTCH_PINTSEL_NDX;



static const uint8_t        SYNC_NODES_MRT_CHANNEL = 1;
static const uint32_t   SYNC_NODES_DELAY_MILLISECS = 2000;

} //namespace tri2b_config

#endif   // ifndef TRI2B_CONFIG_HXX
