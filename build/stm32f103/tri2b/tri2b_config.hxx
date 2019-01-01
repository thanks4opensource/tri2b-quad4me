#ifndef TRI2B_CONFIG_HXX
#define TRI2B_CONFIG_HXX

#include <stdint.h>

#include <stm32f103xb.h>

#include <stm32f10_12357_xx.hxx>


namespace tri2b_config {
using namespace baresil::stm32f10_12357_xx;

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
                        DATA_GPIO_BIT = GPIO_ODR_ODR7;

static const uint8_t    ALRT_GPIO_CRLH_POS = gpio::crl::PORT_6_POS,
                        LTCH_GPIO_CRLH_POS = gpio::crl::PORT_5_POS,
                        DATA_GPIO_CRLH_POS = gpio::crl::PORT_7_POS;



static const uint8_t    ALRT_LTCH_AFIO_EXTICR_NDX = afio::EXTICR2;  // EXTICR[1]

static const uint32_t   ALRT_AFIO_EXTICR_MASK = AFIO_EXTICR2_EXTI6   ,
                        LTCH_AFIO_EXTICR_MASK = AFIO_EXTICR2_EXTI5   ,
                        ALRT_AFIO_EXTICR_PORT = AFIO_EXTICR2_EXTI6_PB,
                        LTCH_AFIO_EXTICR_PORT = AFIO_EXTICR2_EXTI5_PB;

static const uint32_t   ALRT_EXTI_BIT = EXTI_RTSR_TR6,
                        LTCH_EXTI_BIT = EXTI_RTSR_TR5;

static const uint8_t    ALRT_NVIC_IRQ = EXTI9_5_IRQn      ,
                        LTCH_NVIC_IRQ = EXTI9_5_IRQn      ;
static const uint32_t   ALRT_NVIC_BIT = 1 << ALRT_NVIC_IRQ,
                        LTCH_NVIC_BIT = 1 << LTCH_NVIC_IRQ;


static TIM_TypeDef* const   RESET_NODES_TIMER          = TIM4;
static TIM_TypeDef* const   RANDOM_DELAY_TIMER         = TIM4;
static TIM_TypeDef* const   STATISTICS_TIMER_MASTR     = TIM2;
static TIM_TypeDef* const   STATISTICS_TIMER_SLAVE     = TIM3;
static const uint32_t       STATISTICS_TIMER_MSTR_SLAV =   tim_2_5
                                                         ::smcr
                                                         ::TS_MSTR2_SLAV3,
                            SYNC_NODES_DELAY_MILLISECS = 2000;

}  // namespace tri2b_config

#endif   // ifndef TRI2B_CONFIG_HXX
