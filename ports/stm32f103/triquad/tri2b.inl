#include <stm32f103xb.h>
#include <core_cm3.h>   // must be after stm32f103x6.h for NVIC_type

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>
#include <mcu.hxx>

#if defined(TRIQUAD_STATS) || TRIQUAD_MIN_HIGH_US > 0
#include <sys_tick.hxx>
#endif

#include "tri2b.hxx"

#include <tri2b_config.hxx>


namespace tri2b {

#ifdef TRIQUAD_STATS
void Tri2bBase::waits_begn()
{
    _systick_start = arm::SysTick::count();
}

void Tri2bBase::waits_incr()
{
    _waits         += arm::SysTick::elapsed(_systick_start);
    _systick_start  = arm::SysTick::count  (              );
}
#endif // #ifdef TRIQUAD_STATS




#define TRI2B_LINE(ALRT_LTCH_DATA, GPIO_BIT)        \
bool Tri2bBase::ALRT_LTCH_DATA()                    \
volatile const                                      \
{                                                   \
    asm volatile("" ::: "memory");                  \
    return tri2b_config::GPIO->IDR & GPIO_BIT;      \
}

TRI2B_LINE(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_LINE(ltch, tri2b_config::LTCH_GPIO_BIT)
TRI2B_LINE(data, tri2b_config::DATA_GPIO_BIT)

#undef TRI2B_LINE


bool Tri2bBase::alrt_rise()
{
    return  bitops::GET_BITS(EXTI->PR, tri2b_config::ALRT_EXTI_BIT);
}

bool Tri2bBase::ltch_rise()
{
    return bitops::GET_BITS(EXTI->PR, tri2b_config::LTCH_EXTI_BIT);
}


void Tri2bBase::clr_alrt_rise()
{
    EXTI->PR = tri2b_config::ALRT_EXTI_BIT; // writing bit clears it
}

void Tri2bBase::clr_ltch_rise()
{
    EXTI->PR = tri2b_config::LTCH_EXTI_BIT; // writing bit clears it
}



#if TRIQUAD_MIN_HIGH_US == 0

#define TRI2B_SET(ALRT_LTCH, GPIO_BIT)      \
void Tri2bBase::set_##ALRT_LTCH()           \
{                                           \
    asm volatile("" ::: "memory");          \
    tri2b_config::GPIO->BSRR = GPIO_BIT;    \
}
TRI2B_SET(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_SET(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_SET


#define TRI2B_CLR(ALRT_LTCH, GPIO_BIT)      \
void Tri2bBase::clr_##ALRT_LTCH()           \
{                                           \
    asm volatile("" ::: "memory");          \
    tri2b_config::GPIO->BRR = GPIO_BIT;     \
}
TRI2B_CLR(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_CLR(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_CLR

#else  // #if TRIQUAD_MIN_HIGH_US == 0

#define TRI2B_SET(ALRT_LTCH, GPIO_BIT)          \
void Tri2bBase::set_##ALRT_LTCH()               \
{                                               \
    asm volatile("" ::: "memory")          ;    \
    tri2b_config::GPIO->BSRR = GPIO_BIT    ;    \
    _min_high_start = arm::SysTick::count();    \
}
TRI2B_SET(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_SET(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_SET


#define TRI2B_CLR(ALRT_LTCH, GPIO_BIT)                          \
void Tri2bBase::clr_##ALRT_LTCH()                               \
{                                                               \
    while (  arm::SysTick::elapsed(_min_high_start)             \
           <    baresil                                         \
              ::stm32f10_12357_xx                               \
              ::mcu                                             \
              ::microseconds_to_clocks(TRIQUAD_MIN_HIGH_US))    \
        asm("nop");                                             \
                                                                \
    asm volatile("" ::: "memory");                              \
    tri2b_config::GPIO->BRR = GPIO_BIT;                         \
}
TRI2B_CLR(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_CLR(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_CLR

#endif  // #if TRIQUAD_MIN_HIGH_US == 0



#if TRIQUAD_DATA_WAIT_US == 0
void Tri2bBase::set_data()
{
    asm volatile("" ::: "memory")                         ;
    tri2b_config::GPIO->BSRR = tri2b_config::DATA_GPIO_BIT;
}
void Tri2bBase::clr_data() {
    tri2b_config::GPIO->BRR = tri2b_config::DATA_GPIO_BIT;
}
#else
// set_data() non-inline, implemented in tri2b.cxx
void Tri2bBase::clr_data() {
    tri2b_config::GPIO->BRR = tri2b_config::DATA_GPIO_BIT;
    _prev_data = 0;
}
#endif



#ifdef TRIQUAD_INTERRUPTS
void Tri2bBase::disble_alrt_fall()
{
    // disable falling edge detection
    bitops::CLR_BITS(EXTI->FTSR, tri2b_config::ALRT_EXTI_BIT);

    // clear any pending edge detection
    bitops::CLR_BITS(EXTI->PR,  tri2b_config::ALRT_EXTI_BIT);

#if 0  // need to leave enabled to get edge detection
    // disable rising and falling edge trigger interrupt (can't do just falling)
    // set bit to 0 to mask interrupt
    bitops::CLR_BITS(EXTI->IMR, tri2b_config::ALRT_EXTI_BIT);
#endif
}

void Tri2bBase::enable_alrt_fall()
{
    // enable falling edge detection
    bitops::SET_BITS(EXTI->FTSR, tri2b_config::ALRT_EXTI_BIT);

#if 0  // need to leave enabled to get edge detection
    // enable rising and falling edge trigger interrupt (can't do just falling)
    // set bit to 1 to not mask interrupt
    bitops::SET_BITS(EXTI->IMR, tri2b_config::ALRT_EXTI_BIT);
#endif
}
#endif   // ifdef TRIQUAD_INTERRUPTS

} // namespace tri2b
