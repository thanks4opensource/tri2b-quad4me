#include <LPC8xx.h>

#include <bitops.hxx>
#include <lpc8xx.hxx>
#include <mcu.hxx>
#include <mrt.hxx>

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




#define TRI2B_LINE(ALCD_DATA, GPIO_NDX)     \
bool Tri2bBase::ALCD_DATA()                 \
volatile const                              \
{                                           \
    asm volatile("" ::: "memory");          \
    return LPC_GPIO_PORT->B0[GPIO_NDX];     \
}
TRI2B_LINE(alrt, tri2b_config::ALRT_GPIO_NDX)
TRI2B_LINE(ltch, tri2b_config::LTCH_GPIO_NDX)
TRI2B_LINE(data, tri2b_config::DATA_GPIO_NDX)

#undef TRI2B_LINE


bool Tri2bBase::alrt_rise()
{
    return  bitops::GET_BITS(LPC_PIN_INT->RISE, tri2b_config::ALRT_PININT_BIT);
}

bool Tri2bBase::ltch_rise()
{
    return bitops::GET_BITS(LPC_PIN_INT->RISE, tri2b_config::LTCH_PININT_BIT);
}

void Tri2bBase::clr_alrt_rise()
{
    LPC_PIN_INT->RISE = tri2b_config::ALRT_PININT_BIT;  // writing bit clears it
}

void Tri2bBase::clr_ltch_rise()
{
    LPC_PIN_INT->RISE = tri2b_config::LTCH_PININT_BIT;  // writing bit clears it
}

#ifdef TRIQUAD_INTERRUPTS
void Tri2bBase::clr_alrt_fall()
{
    LPC_PIN_INT->FALL = tri2b_config::ALRT_PININT_BIT;  // writing bit clears it
}
#endif




#if TRIQUAD_MIN_HIGH_US == 0

#define TRI2B_SET_CLR(SET_CLR, ALCD, GPIO_NDX, VALUE)   \
void Tri2bBase::SET_CLR##_##ALCD()                      \
{                                                       \
    asm volatile("" ::: "memory");                      \
    LPC_GPIO_PORT->B0[GPIO_NDX] = VALUE;                \
}

TRI2B_SET_CLR(set, alrt, tri2b_config::ALRT_GPIO_NDX, 1)
TRI2B_SET_CLR(set, ltch, tri2b_config::LTCH_GPIO_NDX, 1)
TRI2B_SET_CLR(clr, alrt, tri2b_config::ALRT_GPIO_NDX, 0)
TRI2B_SET_CLR(clr, ltch, tri2b_config::LTCH_GPIO_NDX, 0)
#undef TRI2B_SET_CLR

#else  // #if TRIQUAD_MIN_HIGH_US == 0

#define TRI2B_SET(ALCD, GPIO_NDX)               \
void Tri2bBase::set##_##ALCD()                  \
{                                               \
    asm volatile("" ::: "memory")          ;    \
    LPC_GPIO_PORT->B0[GPIO_NDX] = 1        ;    \
    _min_high_start = arm::SysTick::count();    \
}
TRI2B_SET(alrt, tri2b_config::ALRT_GPIO_NDX)
TRI2B_SET(ltch, tri2b_config::LTCH_GPIO_NDX)
#undef TRI2B_SET

#define TRI2B_CLR(ALCD, GPIO_NDX)                               \
void Tri2bBase::clr##_##ALCD()                                  \
{                                                               \
    while (  arm::SysTick::elapsed(_min_high_start)             \
           <    baresil                                         \
              ::lpc8xx                                          \
              ::mcu                                             \
              ::microseconds_to_clocks(TRIQUAD_MIN_HIGH_US))    \
        asm("nop");                                             \
                                                                \
    asm volatile("" ::: "memory")  ;                            \
    LPC_GPIO_PORT->B0[GPIO_NDX] = 0;                            \
}
TRI2B_CLR(alrt, tri2b_config::ALRT_GPIO_NDX)
TRI2B_CLR(ltch, tri2b_config::LTCH_GPIO_NDX)
#undef TRI2B_CLR

#endif  // #if TRIQUAD_MIN_HIGH_US == 0



#if TRIQUAD_DATA_WAIT_US == 0
void Tri2bBase::set_data() {
    LPC_GPIO_PORT->B0[tri2b_config::DATA_GPIO_NDX] = 1;
}
void Tri2bBase::clr_data() {
    LPC_GPIO_PORT->B0[tri2b_config::DATA_GPIO_NDX] = 0;
}
#else
// set_data() non-inline, implemented in tri2b.cxx
void Tri2bBase::clr_data() {
    LPC_GPIO_PORT->B0[tri2b_config::DATA_GPIO_NDX] = 0;
    _prev_data = 0;
}
#endif


#ifdef TRIQUAD_INTERRUPTS
void Tri2bBase::disble_alrt_fall()
{
    // disable falling edge detection (and interrupt)
    LPC_PIN_INT->CIENF = tri2b_config::ALRT_PININT_BIT;

    // set bit to 1 to clear any pending pin interrupt
    LPC_PIN_INT->FALL = tri2b_config::ALRT_PININT_BIT;
}

void Tri2bBase::enable_alrt_fall()
{
    // enable falling edge detection (and interrupt)
    LPC_PIN_INT->SIENF = tri2b_config::ALRT_PININT_BIT;
}
#endif   // ifdef TRIQUAD_INTERRUPTS


} // namespace tri2b
