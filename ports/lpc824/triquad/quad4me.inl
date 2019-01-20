#include <LPC8xx.h>

#include <bitops.hxx>
#include <lpc8xx.hxx>

#include <mcu.hxx>
#include <mrt.hxx>

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



#define QUAD4ME_SET_CLR(SET_CLR, ALCD, GPIO_NDX, VALUE) \
void Quad4meBase::SET_CLR##_##ALCD()            \
{                                               \
    asm volatile("" ::: "memory");              \
    LPC_GPIO_PORT->B0[GPIO_NDX] = VALUE;        \
}
QUAD4ME_SET_CLR(set, alrt, quad4me_config::ALRT_GPIO_NDX, 1)
QUAD4ME_SET_CLR(set, ltch, quad4me_config::LTCH_GPIO_NDX, 1)
QUAD4ME_SET_CLR(set, cycl, quad4me_config::CYCL_GPIO_NDX, 1)

#if TRIQUAD_DATA_WAIT_US == 0
QUAD4ME_SET_CLR(set, data, quad4me_config::DATA_GPIO_NDX, 1)
#endif

QUAD4ME_SET_CLR(clr, alrt, quad4me_config::ALRT_GPIO_NDX, 0)
QUAD4ME_SET_CLR(clr, ltch, quad4me_config::LTCH_GPIO_NDX, 0)
QUAD4ME_SET_CLR(clr, cycl, quad4me_config::CYCL_GPIO_NDX, 0)
#if TRIQUAD_DATA_WAIT_US > 0
void Quad4meBase::clr_data() {
        LPC_GPIO_PORT->B0[quad4me_config::DATA_GPIO_NDX] = 0;
        _prev_data = 0;
}
#else
QUAD4ME_SET_CLR(clr, data, quad4me_config::DATA_GPIO_NDX, 0)
#endif
#undef QUAD4ME_SET_CLR


#define QUAD4ME_SET_CLR(ALCD_DATA, GPIO_NDX)    \
bool Quad4meBase::ALCD_DATA()                   \
volatile const                                  \
{                                               \
    asm volatile("" ::: "memory");              \
    return LPC_GPIO_PORT->B0[GPIO_NDX];         \
}
QUAD4ME_SET_CLR(alrt, quad4me_config::ALRT_GPIO_NDX)
QUAD4ME_SET_CLR(ltch, quad4me_config::LTCH_GPIO_NDX)
QUAD4ME_SET_CLR(cycl, quad4me_config::CYCL_GPIO_NDX)
QUAD4ME_SET_CLR(data, quad4me_config::DATA_GPIO_NDX)
#undef BITS
#undef QUAD4ME_SET_CLR




#ifdef TRIQUAD_INTERRUPTS
void Quad4meBase::disble_alrt_fall()
{
    // disable falling edge detection (and interrupt)
    LPC_PIN_INT->CIENF = quad4me_config::ALRT_PININT_BIT;

    // set bit to 1 to clear any pending pin interrupt
    LPC_PIN_INT->FALL = quad4me_config::ALRT_PININT_BIT;
}

void Quad4meBase::enable_alrt_fall()
{
    // enable falling edge detection (and interrupt)
    LPC_PIN_INT->SIENF = quad4me_config::ALRT_PININT_BIT;
}

void Quad4meBase::clr_alrt_fall()
{
    LPC_PIN_INT->FALL = quad4me_config::ALRT_PININT_BIT; // writing bit clears it
}
#endif   // ifdef TRIQUAD_INTERRUPTS

} // namespace quad4me
