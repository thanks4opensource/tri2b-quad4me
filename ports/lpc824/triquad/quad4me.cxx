#include <LPC8xx.h>

#include <bitops.hxx>
#include <lpc8xx.hxx>

#include <mcu.hxx>
#include <mrt.hxx>

#include <quad4me_config.hxx>

#include "quad4me.hxx"
#include "quad4me.inl"



// OK if same as e.g. randomtest_port.inl's #ifdef RANDOM_DELAY_US because
// never used at same time
static const uint8_t    MRT_CHANNEL = 0;



namespace quad4me {

// Quad4me derived class-specific methods
//

void Quad4me::init()
{
    Quad4meBase::init();

    open_drain();

#ifdef QUAD4ME_USING_RESETN_AS_GPIO
    bitops::SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL,              // enable
                     sys_con::sys_ahb_clk_ctrl::SWM_BIT);

    bitops::SET_BITS(LPC_SWM->PINENABLE0,                   // disable (sic)
                     swm:: pinenable0::RESET_EN_BIT);

#endif


    // do LPC_SYSCON->IOCONCLKDIV[6-0] programmable glitch filters
    // are 0=="shut down" by default

    // initial state
    set_data();
    set_ltch();
    set_alrt();
    set_cycl();

    // set ports as outputs -- has to be after above sets
    LPC_GPIO_PORT->DIRSET0 =   quad4me_config::DATA_GPIO_BIT
                             | quad4me_config::ALRT_GPIO_BIT
                             | quad4me_config::LTCH_GPIO_BIT
                             | quad4me_config::CYCL_GPIO_BIT;
}


void Quad4me::open_drain()
{
    bitops::SET_BITS(LPC_SYSCON->SYSAHBCLKCTRL,
                     sys_con::sys_ahb_clk_ctrl::IOCON_BIT);

    // mask is single bit wide
#ifndef QUAD4ME_ALRT_PORT_IS_I2C
    *quad4me_config::ALRT_IOCON_PIO_REG |= iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif
#ifndef QUAD4ME_LTCH_PORT_IS_I2C
    *quad4me_config::LTCH_IOCON_PIO_REG |= iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif
#ifndef QUAD4ME_CYCL_PORT_IS_I2C
    *quad4me_config::CYCL_IOCON_PIO_REG |= iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif
#ifndef QUAD4ME_DATA_PORT_IS_I2C
    *quad4me_config::DATA_IOCON_PIO_REG |= iocon::modes::OPEN_DRAIN_ENABLE_BIT;
#endif

    bitops::CLR_BITS(LPC_SYSCON->SYSAHBCLKCTRL,
                     sys_con::sys_ahb_clk_ctrl::IOCON_BIT);
}



// Quad4meBase architecture-specific implemented methods
//

void Quad4meBase::post_reset()
{
}



void Quad4meBase::reset_delay_start()
{
    lpc::MRT::one_shot(MRT_CHANNEL,
                         mcu
                       ::milliseconds_to_clocks(  quad4me_config
                                                ::SYNC_NODES_DELAY_MILLISECS));
}


bool Quad4meBase::reset_delay_wait()
{
    return lpc::MRT::is_running(MRT_CHANNEL);
}


#ifdef TRIQUAD_INTERRUPTS
#undef LEVEL_SENSITIVE   // DEBUG
#define EDGE_SENSITIVE   // DEBUG

void Quad4meBase::enable_interrupt() {
    // map pin interrupts to NVIC interrupts
      LPC_SYSCON->PINTSEL[quad4me_config::ALRT_PINTSEL_NDX]
    = quad4me_config::ALRT_GPIO_NDX;

#ifdef LEVEL_SENSITIVE
    // set to level (vs edge) sensitive
    LPC_PIN_INT->ISEL = quad4me_config::ALRT_PININT_BIT;
#endif

#ifdef LEVEL_SENSITIVE
    // enable line low detection (and interrupt)
    // IENF controls (IENR not used for level detection)
    // bit==1 means high detection, ==0 low
    LPC_PIN_INT->CIENF = quad4me_config::ALRT_PININT_BIT;
#endif

#ifdef EDGE_SENSITIVE
    LPC_PIN_INT->SIENF = quad4me_config::ALRT_PININT_BIT;
#endif

    // clear any pending NVIC interrupt
    NVIC->ICPR[0] = quad4me_config::ALRT_NVIC_BIT;

    // enable NVIC interrupt
    NVIC->ISER[0] = quad4me_config::ALRT_NVIC_BIT;

#undef LEVEL_SENSITIVE
#undef EDGE_SENSITIVE
}
#endif   // ifdef TRIQUAD_INTERRUPTS





// Quad4meBase architecture-specific implemented methods
//

#if TRIQUAD_DATA_WAIT_US != 0
void Quad4meBase::set_data() {
    LPC_GPIO_PORT->B0[quad4me_config::DATA_GPIO_NDX] = 1;

    static const uint32_t
      COUNTS
    = baresil::lpc8xx::mcu::microseconds_to_clocks(TRIQUAD_DATA_WAIT_US) / 10;
      // estimate of clocks/loop, -O1

    if (_phase == Phase::IDLE || _phase == Phase::ARBT) {
        uint32_t    count = COUNTS;

        // need timout: other node(s) might be pulling line down
        for ( ; !data() && count ; --count) {
#ifdef TRIQUAD_STATS
            ++_data_waits;
#endif
            asm("nop");
        }

        if (!count) {
              _data_waits -= COUNTS;     // separate waits from timeouts
            ++_data_timeouts      ;
        }
    }
    else if (!_prev_data)  // only if was previously set low
        while (!data()) {
#ifdef TRIQUAD_STATS
            ++_data_waits;
#endif
            asm("nop");
        }

    _prev_data = 1;  // always do, for last bit of ARBT to first of META
}
#endif  // #if TRIQUAD_DATA_WAIT_US != 0




} // namespace quad4me
