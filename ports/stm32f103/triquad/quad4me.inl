#include <stm32f103xb.h>
#include <core_cm3.h>   // must be after stm32f103x6.h for NVIC_type

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <mcu.hxx>

#ifdef TRIQUAD_STATS
#include <sys_tick.hxx>
#endif

#include "quad4me.hxx"

#include <quad4me_config.hxx>


namespace quad4me {

#ifdef TRIQUAD_STATS
void Quad4meBase::waits_begn()
{
    _systick_start = arm::SysTick::count();
}

void Quad4meBase::waits_incr()
{
    _waits         += arm::SysTick::elapsed(_systick_start);
    _systick_start  = arm::SysTick::count  (              );
}
#endif // #ifdef TRIQUAD_STATS



#define QUAD4ME_LINE(ALRT_LTCH_DATA, GPIO_BIT)      \
bool Quad4meBase::ALRT_LTCH_DATA()                  \
volatile const                                      \
{                                                   \
    asm volatile("" ::: "memory");                  \
    return quad4me_config::GPIO->IDR & GPIO_BIT;    \
}

QUAD4ME_LINE(alrt, quad4me_config::ALRT_GPIO_BIT)
QUAD4ME_LINE(ltch, quad4me_config::LTCH_GPIO_BIT)
QUAD4ME_LINE(cycl, quad4me_config::CYCL_GPIO_BIT)
QUAD4ME_LINE(data, quad4me_config::DATA_GPIO_BIT)
#undef BITS
#undef QUAD4ME_LINE



// warning: hardcoded
#define QUAD4ME_SET(ALRT_LTCH, GPIO_BIT)    \
void Quad4meBase::set_##ALRT_LTCH()         \
{                                           \
    asm volatile("" ::: "memory");          \
    quad4me_config::GPIO->BSRR = GPIO_BIT;  \
    while (!(GPIOB->ODR & GPIO_BIT))        \
        asm("nop");                         \
}
QUAD4ME_SET(alrt, quad4me_config::ALRT_GPIO_BIT)
QUAD4ME_SET(ltch, quad4me_config::LTCH_GPIO_BIT)
QUAD4ME_SET(cycl, quad4me_config::CYCL_GPIO_BIT)
#if TRIQUAD_DATA_WAIT_US == 0
QUAD4ME_SET(data, quad4me_config::DATA_GPIO_BIT)
#endif
#undef QUAD4ME_SET

// warning: hardcoded
#define QUAD4ME_CLR(ALRT_LTCH, GPIO_BIT)    \
void Quad4meBase::clr_##ALRT_LTCH()         \
{                                           \
    asm volatile("" ::: "memory");          \
    quad4me_config::GPIO->BRR = GPIO_BIT;   \
    while (GPIOB->ODR & GPIO_BIT)           \
        asm("nop");                         \
}
QUAD4ME_CLR(alrt, quad4me_config::ALRT_GPIO_BIT)
QUAD4ME_CLR(ltch, quad4me_config::LTCH_GPIO_BIT)
QUAD4ME_CLR(cycl, quad4me_config::CYCL_GPIO_BIT)
#if TRIQUAD_DATA_WAIT_US > 0
void Quad4meBase::clr_data() {
    quad4me_config::GPIO->BRR = quad4me_config::DATA_GPIO_BIT;
    _prev_data = 0;
}
#else
QUAD4ME_CLR(data, quad4me_config::DATA_GPIO_BIT)
#endif
#undef QUAD4ME_CLR



#ifdef TRIQUAD_INTERRUPTS
void Quad4meBase::disble_alrt_fall()
{
    // enable rising and falling edge trigger interrupt
    // set bit to 1 to not mask interrupt
    bitops::SET_BITS(EXTI->IMR, quad4me_config::ALRT_EXTI_BIT);

    // clear any pending edge detection
    EXTI->PR = quad4me_config::ALRT_EXTI_BIT; // writing bit clears it
}

void Quad4meBase::enable_alrt_fall()
{
    // enable rising and falling edge trigger interrupt
    // set bit to 1 to not mask interrupt
    bitops::SET_BITS(EXTI->IMR, quad4me_config::ALRT_EXTI_BIT);
}


void Quad4meBase::clr_alrt_fall()
{
    EXTI->PR = quad4me_config::ALRT_EXTI_BIT; // writing bit clears it
}
#endif   // ifdef TRIQUAD_INTERRUPTS

} // namespace quad4me

