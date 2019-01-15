#include <LPC8xx.h>

#include <bitops.hxx>
#include <lpc8xx.hxx>

#include <mcu.hxx>
#include <mrt.hxx>

#if TRIQUAD_MIN_HIGH_US > 0 || TRIQUAD_DATA_WAIT_US > 0
#include <sys_tick.hxx>
#endif

#include <tri2b_config.hxx>

#include "tri2b.hxx"
#include "tri2b.inl"



// OK if same as e.g. randomtest_port.inl's #ifdef RANDOM_DELAY_US because
// never used at same time
static const uint8_t    MRT_CHANNEL = 0;



namespace tri2b {

// Tri2b derived class-specific methods
//

void Tri2b::init()
{
    Tri2bBase::init();


    // enable writing to IOCON registers
    bitops::SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL,
                     sys_con::sys_ahb_clk_ctrl::IOCON_BIT);

    // set lines to open-drain
    // mask is single bit wide
#ifndef TRI2B_ALRT_PORT_IS_I2C
      *tri2b_config::ALRT_IOCON_PIO_REG
    =   iocon::modes::NO_PULL_BITS
      | iocon::modes::HYSTERESIS_BIT
      | iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif
#ifndef TRI2B_LTCH_PORT_IS_I2C
      *tri2b_config::LTCH_IOCON_PIO_REG
    =   iocon::modes::NO_PULL_BITS
      | iocon::modes::HYSTERESIS_BIT
      | iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif
#ifndef TRI2B_DATA_PORT_IS_I2C
      *tri2b_config::DATA_IOCON_PIO_REG
    =   iocon::modes::NO_PULL_BITS
      | iocon::modes::HYSTERESIS_BIT
      | iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif


#if TRIQUAD_MIN_HIGH_US > 0 && defined(TRIQUAD_NXP_GLITCH_FILTER)
    // LPC_SYSCON->IOCONCLKDIV[6-0] programmable glitch filters
    // are 0=="shut down" by default

    const uint32_t  GLITCH_CLKS =      baresil
                                     ::lpc8xx
                                     ::mcu
                                     ::microseconds_to_clocks(TRIQUAD_MIN_HIGH_US)
                                   / 2;

    uint32_t    GLITCH_DIV,
                GLITCH_MUL;

    if (GLITCH_CLKS > 3 * 255) {
        // max achievable
        GLITCH_DIV = 255;
        GLITCH_MUL = 3  ;
    }
    else if (GLITCH_CLKS > 2 * 255) {
        GLITCH_DIV = GLITCH_CLKS / 3;
        GLITCH_MUL = 3  ;
    }
    else if (GLITCH_CLKS > 255) {
        GLITCH_DIV = GLITCH_CLKS / 2;
        GLITCH_MUL = 2              ;
    }
    else {
        GLITCH_DIV = GLITCH_CLKS;
        GLITCH_MUL = 1          ;
    }

    *tri2b_config::GLITCH_SYSCON_CLKDIV_REG = GLITCH_DIV;

    // enable IOCON registers
    bitops::SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL,
                     sys_con::sys_ahb_clk_ctrl::IOCON_BIT);

    // same bits/values for both I2C and normal ports
    bitops::SET_BITS(*tri2b_config::ALRT_IOCON_PIO_REG      ,
                       GLITCH_MUL << iocon::modes::FILTER_MODE_POS
                     | tri2b_config::GLITCH_SYSCON_DIV            );
    bitops::SET_BITS(*tri2b_config::LTCH_IOCON_PIO_REG      ,
                       GLITCH_MUL << iocon::modes::FILTER_MODE_POS
                     | tri2b_config::GLITCH_SYSCON_DIV            );

#endif  // #if TRIQUAD_MIN_HIGH_US > 0 && defined(TRIQUAD_NXP_GLITCH_FILTER)


    // disable writing to IOCON registers
    bitops::CLR_BITS(LPC_SYSCON->SYSAHBCLKCTRL,
                     sys_con::sys_ahb_clk_ctrl::IOCON_BIT);



#ifdef TRI2B_USING_RESETN_AS_GPIO
    bitops::SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL,              // enable
                     sys_con::sys_ahb_clk_ctrl::SWM_BIT);

    bitops::SET_BITS(LPC_SWM->PINENABLE0,                   // disable (sic)
                     swm:: pinenable0::RESET_EN_BIT);

#endif


    // map pin interrupts to NVIC interrupts
      LPC_SYSCON->PINTSEL[tri2b_config::ALRT_PINTSEL_NDX]
    = tri2b_config::ALRT_GPIO_NDX;
      LPC_SYSCON->PINTSEL[tri2b_config::LTCH_PINTSEL_NDX]
    = tri2b_config::LTCH_GPIO_NDX;


    // alrt, ltch and data all low here
    set_data();
    set_ltch();
    set_alrt();

    // set ports as outputs -- has to be after above sets
    LPC_GPIO_PORT->DIRSET0 =   tri2b_config::DATA_GPIO_BIT
                             | tri2b_config::ALRT_GPIO_BIT
                             | tri2b_config::LTCH_GPIO_BIT;

#if defined(TRIQUAD_STATS) || TRIQUAD_MIN_HIGH_US > 0 || TRIQUAD_DATA_WAIT_US > 0
    // not really one shot, will run continuously
    arm::SysTick::one_shot(arm::SysTick::MAX_COUNTS);
#endif
}



// Tri2bBase architecture-specific implemented methods
//

void Tri2bBase::post_reset()
{
}



void Tri2bBase::reset_delay_start()
{
    lpc::MRT::one_shot(MRT_CHANNEL,
                         mcu
                       ::milliseconds_to_clocks(  tri2b_config
                                                ::SYNC_NODES_DELAY_MILLISECS));
}


bool Tri2bBase::reset_delay_wait()
{
    return lpc::MRT::is_running(MRT_CHANNEL);
}


#ifdef TRIQUAD_INTERRUPTS

void Tri2bBase::enable_interrupt() {
    // map pin interrupts to NVIC interrupts
      LPC_SYSCON->PINTSEL[tri2b_config::ALRT_PINTSEL_NDX]
    = tri2b_config::ALRT_GPIO_NDX;

    // clear any pending NVIC interrupt
    NVIC->ICPR[0] = tri2b_config::ALRT_NVIC_BIT;

    // enable NVIC interrupt
    NVIC->ISER[0] = tri2b_config::ALRT_NVIC_BIT;
}
#endif  // ifdef TRIQUAD_INTERRUPTS



#if TRIQUAD_DATA_WAIT_US > 0
void Tri2bBase::set_data() {
    LPC_GPIO_PORT->B0[tri2b_config::DATA_GPIO_NDX] = 1;

    uint32_t    systick_start = arm::SysTick::count();

    if (_phase == Phase::IDLE || _phase == Phase::ARBT) {
        // need timout: other node(s) might be pulling line down
        while (   !data()
               &&   arm::SysTick::elapsed(systick_start)
                  < baresil::lpc8xx::mcu::microseconds_to_clocks(
                                          TRIQUAD_DATA_WAIT_US   ))
            asm("nop");
#ifdef TRIQUAD_STATS
        if (!data()) ++_data_timeouts;
#endif
    }
    else if (!_prev_data)  // only if was previously set low
        while (!data())
            asm("nop");

#ifdef TRIQUAD_STATS
        _data_waits += arm::SysTick::elapsed(systick_start);
#endif

    _prev_data = 1;  // always do, for last bit of ARBT to first of META
}
#endif  // #if TRIQUAD_DATA_WAIT_US > 0




} // namespace tri2b
