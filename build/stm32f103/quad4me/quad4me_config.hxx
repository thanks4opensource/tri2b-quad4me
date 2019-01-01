#ifndef QUAD4ME_CONFIG_HXX
#define QUAD4ME_CONFIG_HXX

#include <stdint.h>

#include <stm32f103xb.h>

#include <stm32f10_12357_xx.hxx>


namespace quad4me_config {
using namespace baresil::stm32f10_12357_xx;

static const uint8_t    NUM_ARBT_BITS  = 3,
                        NUM_META_BITS  = 2,
                        NUM_META_BYTES = 1;


#ifdef TRIQUAD_INTERRUPTS
// must match ALRT_EXTI_xxx and ALRT_NVIC_xxx below
static const IRQn_Type      ALRT_IRQN = EXTI9_5_IRQn;
#define TRI2B_ALRT_ISR      EXTI9_5_IRQHandler
#endif

static          GPIO_TypeDef* const     GPIO     = GPIOB      ;
static volatile uint32_t*     const     GPIO_CR = &GPIOB->CRL;

// following must match
//
static const uint32_t   ALRT_GPIO_BIT = GPIO_ODR_ODR6,
                        LTCH_GPIO_BIT = GPIO_ODR_ODR5,
                        CYCL_GPIO_BIT = GPIO_ODR_ODR1,
                        DATA_GPIO_BIT = GPIO_ODR_ODR7;

static const uint8_t    ALRT_GPIO_CRLH_POS = gpio::crl::PORT_6_POS,
                        LTCH_GPIO_CRLH_POS = gpio::crl::PORT_5_POS,
                        CYCL_GPIO_CRLH_POS = gpio::crl::PORT_1_POS,
                        DATA_GPIO_CRLH_POS = gpio::crl::PORT_7_POS;



#ifdef TRIQUAD_INTERRUPTS
#define QUAD4ME_ALRT_ISR    EXTI9_5_IRQHandler

static const uint8_t    ALRT_LTCH_AFIO_EXTICR_NDX = afio::EXTICR2;  // EXTICR[1]

static const uint32_t   ALRT_AFIO_EXTICR_MASK = AFIO_EXTICR2_EXTI6   ,
                        LTCH_AFIO_EXTICR_MASK = AFIO_EXTICR2_EXTI5   ,
// need EXTICR1?        CYCL_AFIO_EXTICR_MASK = AFIO_EXTICR2_EXTI1   ,
                        ALRT_AFIO_EXTICR_PORT = AFIO_EXTICR2_EXTI6_PB,
                        LTCH_AFIO_EXTICR_PORT = AFIO_EXTICR2_EXTI5_PB;
// need EXTICR1?        CYCL_AFIO_EXTICR_PORT = AFIO_EXTICR2_EXTI1_PB;

static const uint32_t   ALRT_EXTI_BIT = EXTI_RTSR_TR6,
                        LTCH_EXTI_BIT = EXTI_RTSR_TR5,
                        CYCL_EXTI_BIT = EXTI_RTSR_TR1;

static const uint8_t    ALRT_NVIC_IRQ = EXTI9_5_IRQn      ,
                        LTCH_NVIC_IRQ = EXTI9_5_IRQn      ;
static const uint32_t   ALRT_NVIC_BIT = 1 << ALRT_NVIC_IRQ,
                        LTCH_NVIC_BIT = 1 << LTCH_NVIC_IRQ;
#endif


static TIM_TypeDef* const   RESET_NODES_TIMER          = TIM4;
static TIM_TypeDef* const   RANDOM_DELAY_TIMER         = TIM4;
static TIM_TypeDef* const   STATISTICS_TIMER_MASTR     = TIM2;
static TIM_TypeDef* const   STATISTICS_TIMER_SLAVE     = TIM3;
static const uint32_t       STATISTICS_TIMER_MSTR_SLAV =   tim_2_5
                                                         ::smcr
                                                         ::TS_MSTR2_SLAV3,
                            SYNC_NODES_DELAY_MILLISECS = 2000;

}  // namespace quad4me_config

#endif   // ifndef QUAD4ME_CONFIG_HXX
