#ifndef QUAD4ME_CONFIG_HXX
#define QUAD4ME_CONFIG_HXX

#include <stdint.h>

#include <LPC82x.h>

#include <lpc8xx.hxx>


using namespace baresil::lpc8xx;

namespace quad4me_config {

static const uint8_t    NUM_ARBT_BITS  = 3,
                        NUM_META_BITS  = 2,
                        NUM_META_BYTES = 1;


#ifdef TRIQUAD_INTERRUPTS
// must match
static const IRQn_Type      ALRT_IRQN = PININT4_IRQn,
                            LTCH_IRQN = PININT5_IRQn,
                            STRT_IRQN = Reserved11_IRQn;
#define QUAD4ME_ALRT_ISR    PININT4_IRQHandler
#define QUAD4ME_LTCH_ISR    PININT5_IRQHandler
#define QUAD4ME_STRT_ISR    Reserved1_IRQHandler
static const uint32_t       ALRT_NVIC_BIT = nvic::PININT4_BIT,
                            LTCH_NVIC_BIT = nvic::PININT5_BIT,
                            STRT_NVIC_BIT = nvic::RESERVED1_BIT;
#endif

// special open drain ports -- don't try to set open drain in IOCON
#define QUAD4ME_DATA_PORT_IS_I2C
#define QUAD4ME_ALRT_PORT_IS_I2C
#undef  QUAD4ME_LTCH_PORT_IS_I2C
#undef  QUAD4ME_CYCL_PORT_IS_I2C

#undef QUAD4ME_USING_RESETN_AS_GPIO

static volatile uint32_t* const     ALRT_IOCON_PIO_REG = &LPC_IOCON->PIO0_10;
static volatile uint32_t* const     LTCH_IOCON_PIO_REG = &LPC_IOCON-> PIO0_4;
static volatile uint32_t* const     CYCL_IOCON_PIO_REG = &LPC_IOCON-> PIO0_8;
static volatile uint32_t* const     DATA_IOCON_PIO_REG = &LPC_IOCON->PIO0_11;

                        // match
static const uint8_t    ALRT_GPIO_NDX = gpio::PIN_10_POS,   // I2C0 clock
                        LTCH_GPIO_NDX = gpio:: PIN_4_POS,   // ~TRST
                        CYCL_GPIO_NDX = gpio:: PIN_8_POS,
                        DATA_GPIO_NDX = gpio::PIN_11_POS;   // I2C0 data
static const uint32_t   ALRT_GPIO_BIT = gpio::PIN_10_BIT,   // I2C0 clock
                        LTCH_GPIO_BIT = gpio:: PIN_4_BIT,   // ~TRST
                        CYCL_GPIO_BIT = gpio:: PIN_8_BIT,
                        DATA_GPIO_BIT = gpio::PIN_11_BIT;   // I2C0 data

                        // match XXXX_IRQN above
static const uint8_t    ALRT_PINTSEL_NDX = 4,
                        LTCH_PINTSEL_NDX = 5,
                        CYCL_PINTSEL_NDX = 6;

static const uint8_t    ALRT_PININT_BIT = 1 << ALRT_PINTSEL_NDX,
                        LTCH_PININT_BIT = 1 << LTCH_PINTSEL_NDX,
                        CYCL_PININT_BIT = 1 << CYCL_PINTSEL_NDX;



static const uint8_t        SYNC_NODES_MRT_CHANNEL = 1;
static const uint32_t   SYNC_NODES_DELAY_MILLISECS = 2000;

} //namespace quad4me_config

#endif   // ifndef QUAD4ME_CONFIG_HXX
