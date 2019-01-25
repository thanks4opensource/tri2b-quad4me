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


namespace triquad {

#ifdef TRIQUAD_STATS
void TriQuad::waits_begn()
{
    _systick_start = arm::SysTick::count();
}

void TriQuad::waits_incr()
{
    _waits         += arm::SysTick::elapsed(_systick_start);
    _systick_start  = arm::SysTick::count  (              );
}
#endif // #ifdef TRIQUAD_STATS




#define TRI2B_LINE(ALRT_LTCH_DATA, GPIO_BIT)        \
bool TriQuad::ALRT_LTCH_DATA()                  \
volatile const                                      \
{                                                   \
    asm volatile("" ::: "memory");                  \
    return tri2b_config::GPIO->IDR & GPIO_BIT;      \
}

TRI2B_LINE(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_LINE(ltch, tri2b_config::LTCH_GPIO_BIT)
TRI2B_LINE(data, tri2b_config::DATA_GPIO_BIT)

#undef TRI2B_LINE


bool TriQuad::alrt_rise()
{
    return  bitops::GET_BITS(EXTI->PR, tri2b_config::ALRT_EXTI_BIT);
}

bool TriQuad::ltch_rise()
{
    return bitops::GET_BITS(EXTI->PR, tri2b_config::LTCH_EXTI_BIT);
}


void TriQuad::clr_alrt_rise()
{
    EXTI->PR = tri2b_config::ALRT_EXTI_BIT; // writing bit clears it
}

void TriQuad::clr_ltch_rise()
{
    EXTI->PR = tri2b_config::LTCH_EXTI_BIT; // writing bit clears it
}

#ifdef TRIQUAD_INTERRUPTS
void TriQuad::clr_alrt_fall()
{
    EXTI->PR = tri2b_config::ALRT_EXTI_BIT; // writing bit clears it
}
#endif




#if TRIQUAD_MIN_HIGH_US == 0

#define TRI2B_SET(ALRT_LTCH, GPIO_BIT)      \
void TriQuad::set_##ALRT_LTCH()         \
{                                           \
    asm volatile("" ::: "memory");          \
    tri2b_config::GPIO->BSRR = GPIO_BIT;    \
}
TRI2B_SET(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_SET(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_SET


#define TRI2B_CLR(ALRT_LTCH, GPIO_BIT)      \
void TriQuad::clr_##ALRT_LTCH()         \
{                                           \
    asm volatile("" ::: "memory");          \
    tri2b_config::GPIO->BRR = GPIO_BIT;     \
}
TRI2B_CLR(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_CLR(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_CLR

#else  // #if TRIQUAD_MIN_HIGH_US == 0

void Tri2bBase::start_min_high()
{
    _min_high_start = arm::SysTick::count();
}

#define TRI2B_SET(ALRT_LTCH, GPIO_BIT)                  \
void TriQuad::set_##ALRT_LTCH()                         \
{                                                       \
    asm volatile("" ::: "memory")                  ;    \
    tri2b_config::GPIO->BSRR = GPIO_BIT            ;    \
    static_cast<Tri2bBase*>(this)->start_min_high();    \
}
TRI2B_SET(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_SET(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_SET

void Tri2bBase::wait_min_high()
{
    while (  arm::SysTick::elapsed(_min_high_start)
           <    baresil
              ::stm32f10_12357_xx
              ::mcu
              ::microseconds_to_clocks(TRIQUAD_MIN_HIGH_US))
        asm("nop");
}


#define TRI2B_CLR(ALRT_LTCH, GPIO_BIT)                  \
void TriQuad::clr_##ALRT_LTCH()                         \
{                                                       \
    static_cast<Tri2bBase*>(this)->wait_min_high();     \
                                                        \
    asm volatile("" ::: "memory");                      \
    tri2b_config::GPIO->BRR = GPIO_BIT;                 \
}
TRI2B_CLR(alrt, tri2b_config::ALRT_GPIO_BIT)
TRI2B_CLR(ltch, tri2b_config::LTCH_GPIO_BIT)
#undef TRI2B_CLR

#endif  // #if TRIQUAD_MIN_HIGH_US == 0



#if TRIQUAD_DATA_WAIT_US == 0
void TriQuad::set_data()
{
    asm volatile("" ::: "memory")                         ;
    tri2b_config::GPIO->BSRR = tri2b_config::DATA_GPIO_BIT;
}
void TriQuad::clr_data() {
    tri2b_config::GPIO->BRR = tri2b_config::DATA_GPIO_BIT;
}
#else
// set_data() non-inline, implemented in tri2b.cxx
void TriQuad::clr_data() {
    tri2b_config::GPIO->BRR = tri2b_config::DATA_GPIO_BIT;
    _prev_data = 0;
}
#endif



#ifdef TRIQUAD_INTERRUPTS
void TriQuad::disble_alrt_fall()
{
    // disable falling edge detection
    bitops::CLR_BITS(EXTI->FTSR, tri2b_config::ALRT_EXTI_BIT);

    // clear any pending edge detection
    EXTI->PR = tri2b_config::ALRT_EXTI_BIT;   // writing bit clears it

#if 0  // need to leave enabled to get edge detection
    // disable rising and falling edge trigger interrupt (can't do just falling)
    // set bit to 0 to mask interrupt
    bitops::CLR_BITS(EXTI->IMR, tri2b_config::ALRT_EXTI_BIT);
#endif
}

void TriQuad::enable_alrt_fall()
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

} // namespace triquad
