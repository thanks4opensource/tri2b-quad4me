#include <LPC8xx.h>

#include <lpc8xx.hxx>

#include <sct.hxx>

using namespace baresil::lpc8xx;

/*
/usr/local/doc/hardware/nxp/AN11538_SCTimer_PWM_Cookbook_v5.0.pdf
/usr/local/doc/hardware/nxp/AN11538_code_examples/LPC82x/LPCOpen
./applications/lpc82x/examples/SCTimer_PWM/periph_SCT_rc5_send/sct_user.h
./applications/lpc82x/examples/SCTimer_PWM/periph_SCT_rc5_send/the_real_sct_user.h
./applications/lpc82x/examples/SCTimer_PWM/periph_SCT_rc5_send/sct_fsm.h
./applications/lpc82x/examples/SCTimer_PWM/periph_SCT_rc5_receive/sct_user.h
./applications/lpc82x/examples/SCTimer_PWM/periph_SCT_rc5_receive/the_real_sct_user.h
./software/lpc_core/lpc_chip/chip_8xx/sct_pwm_8xx.h
./software/lpc_core/lpc_chip/chip_8xx/sct_8xx.h
./software/lpc_core/lpc_chip/chip_8xx/sct_8xx.c
./software/lpc_core/lpc_chip/chip_8xx/sct_pwm_8xx.c
file:///usr/local/doc/hardware/nxp/lpcopen/lpcopen_2_19_docs/sct__8xx_8h_source.html
file:///usr/local/doc/hardware/nxp/lpcopen/lpcopen_2_19_docs/sct__8xx_8c_source.html
file:///usr/local/doc/hardware/nxp/lpcopen/lpcopen_2_19_docs/sct__pwm__8xx_8h_source.html
*/




namespace lpc {

bool SCT::is_running()
{
    return !(LPC_SCT->EVFLAG & sct::evflag::EVENT_0_BIT);
}



uint32_t SCT::count()
{
    return LPC_SCT->COUNT;
}



void SCT::one_shot(
uint32_t    counts,
bool        interrupt)
{
    if (counts == 0) return;

    LPC_SCT->CONFIG =   sct::config::UNIFY_BIT
                      | sct::config::CLKMODE_SYSTEM_BITS
                      | sct::config::NORELOAD_L_BIT
                      | sct::config::NORELOAD_H_BIT
                      | sct::config::AUTOLIMIT_L_BIT
                      | sct::config::AUTOLIMIT_H_BIT;

    LPC_SCT->CTRL = sct::ctrl::HALT_U_BIT;

    LPC_SCT->EVFLAG |= sct::evflag::EVENT_0_BIT;  // clear timed-out flag

    LPC_SCT->EVENT[0].STATE = sct::event::state::STATEMASK_MASK;// enable all
    LPC_SCT->EVENT[0].CTRL  = sct::event::ctrl  ::COMBMODE_MATCH_BITS;

    LPC_SCT->MATCH[0].U = counts;  // set limit

    // clear counter, set scaling, and un-halt
    LPC_SCT->CTRL       =   sct::ctrl::CLRCTR_U_BIT
                          | ((_prescale - 1) << sct::ctrl::PRESCALE_U_POS);
}


void SCT::pause()
{
    LPC_SCT->CTRL |= sct::ctrl::HALT_U_BIT;
}


void SCT::resume()
{
    LPC_SCT->CTRL &= ~sct::ctrl::HALT_U_BIT;
}


void SCT::busy_delay(
uint32_t    counts,
bool        interrupt)
{
    one_shot(counts, interrupt);

    while (is_running())
         asm("nop");
}

} // namespace lpc
