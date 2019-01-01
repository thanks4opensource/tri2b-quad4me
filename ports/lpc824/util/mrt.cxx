#include <LPC8xx.h>

#include <lpc8xx.hxx>

#include <mrt.hxx>

using namespace baresil::lpc8xx;



namespace lpc {

bool MRT::is_running(
uint8_t     channel)
{
    return    (LPC_MRT->Channel[channel].STAT & mrt::status::IS_RUNNING_BIT)
           ==                                   mrt::status::IS_RUNNING_BIT;
}



uint8_t MRT::get_idle_channel()
{
    // returns (channel) 0 to 3 on success, 4 on failure
    return    (LPC_MRT->IDLE_CH & mrt::idle_ch::CHAN_MASK)
           >> mrt::idle_ch::CHAN_POS;
}



void MRT::one_shot(
uint8_t     channel,
uint32_t    counts,
bool        interrupt)
{
    if (counts == 0) return;

    LPC_MRT->Channel[channel].CTRL = mrt::ctrl::ONE_SHOT_INTERRUPT_BITS;

    LPC_MRT->Channel[channel].INTVAL =   (counts & MAX_COUNTS)
                                       | mrt::interval_value::FORCE_LOAD_BIT;
}



void MRT::busy_delay(
uint8_t     channel,
uint32_t    counts,
bool        interrupt)
{
    one_shot(channel, counts, interrupt);

    while (is_running(channel))
         asm("nop");
}

} // namespace lpc
