#include <stm32f103xb.h>
#include <core_cm3.h>   // must be after stm32f103x6.h for NVIC_type

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <mcu.hxx>
#include <tim.hxx>

#if TRIQUAD_MIN_HIGH_US > 0 || TRIQUAD_DATA_WAIT_US > 0
#include <sys_tick.hxx>
#endif

#include <quad4me_config.hxx>

#include "quad4me.hxx"
#include "quad4me.inl"



namespace {
    stm32f10_12357_xx::TIM      reset_timer;
}

using namespace baresil::stm32f10_12357_xx;



namespace quad4me {

// Quad4me derived class-specific methods
//

void Quad4me::init()
{
    Quad4meBase::init();

    // initial state
    set_data();
    set_ltch();
    set_alrt();
    set_cycl();

    // set pins to GPIO output, 2MHz max
    bitops::CHG_BITS(*quad4me_config::GPIO_CR                                 ,

                       (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << quad4me_config::ALRT_GPIO_CRLH_POS                )
                     | (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << quad4me_config::LTCH_GPIO_CRLH_POS                )
                     | (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << quad4me_config::CYCL_GPIO_CRLH_POS                )
                     | (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << quad4me_config::DATA_GPIO_CRLH_POS                ),

                       (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << quad4me_config::ALRT_GPIO_CRLH_POS                )
                     | (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << quad4me_config::LTCH_GPIO_CRLH_POS                )
                     | (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << quad4me_config::CYCL_GPIO_CRLH_POS                )
                     | (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << quad4me_config::DATA_GPIO_CRLH_POS                ));

#ifdef TRIQUAD_INTERRUPTS
    // set up interrupts / edge detection
    //

    // mapping of pins to interrupts (RCC_APB2ENR_AFIOEN needs to be
    //   permanently enabled in mcu::init() -- can't enable here,
    //   set AFIO->EXTICR, then disable
    bitops::CHG_BITS(AFIO->EXTICR[quad4me_config::ALRT_LTCH_AFIO_EXTICR_NDX],
                     quad4me_config::ALRT_AFIO_EXTICR_MASK                  ,
                     quad4me_config::ALRT_AFIO_EXTICR_PORT                   );

    // enable falling edge detection
    bitops::SET_BITS(EXTI->FTSR, quad4me_config::ALRT_EXTI_BIT);
#endif

#if TRIQUAD_DATA_WAIT_US > 0
    // not really one shot, will run continuously
    arm::SysTick::one_shot(arm::SysTick::MAX_COUNTS);
#endif
}



// Quad4meBase architecture-specific implemented methods
//

void Quad4meBase::post_reset()
{
    if (quad4me_config::RANDOM_DELAY_TIMER == quad4me_config::RESET_NODES_TIMER)
        // reset timebase because reusing same timer

        reset_timer.init(quad4me_config::RANDOM_DELAY_TIMER,
                         static_cast<uint16_t>(  baresil
                                               ::stm32f10_12357_xx
                                               ::mcu
                                               ::microseconds_to_clocks(1)));
}



void Quad4meBase::reset_delay_start()
{
    reset_timer.init(quad4me_config::RESET_NODES_TIMER,
                     static_cast<uint16_t>(  baresil
                                           ::stm32f10_12357_xx
                                           ::mcu
                                           ::milliseconds_to_clocks(1)));

    reset_timer.one_shot(quad4me_config::SYNC_NODES_DELAY_MILLISECS);
}


bool Quad4meBase::reset_delay_wait()
{
    return reset_timer.is_running();
}



#ifdef TRIQUAD_INTERRUPTS
void Quad4meBase::enable_interrupt() {
    // clear any pending edge detextion
    bitops::CLR_BITS(EXTI->PR,   quad4me_config::ALRT_EXTI_BIT);

    // enable rising and falling edge trigger interrupt
    // set bit to 1 to not mask interrupt
    bitops::SET_BITS(EXTI->IMR, quad4me_config::ALRT_EXTI_BIT);

    // clear any pending interrupt
    NVIC->ICPR[0] = quad4me_config::ALRT_NVIC_BIT;

    // enable interrupt
    NVIC->ISER[0] = quad4me_config::ALRT_NVIC_BIT;
}
#endif   // ifdef TRIQUAD_INTERRUPTS



#if TRIQUAD_DATA_WAIT_US > 0
void Quad4meBase::set_data() {
    quad4me_config::GPIO->BSRR = quad4me_config::DATA_GPIO_BIT;

    uint32_t    systick_start = arm::SysTick::count();

    if (_phase == Phase::IDLE || _phase == Phase::ARBT) {
#ifdef TRIQUAD_STATS
        uint32_t    prev_data_waits = _data_waits;
#endif
        // need timout: other node(s) might be pulling line down
        while (   !data()
               &&   arm::SysTick::elapsed(systick_start)
                  < baresil::stm32f10_12357_xx::mcu::microseconds_to_clocks(
                                                     TRIQUAD_DATA_WAIT_US   )) {
#ifdef TRIQUAD_STATS
            ++_data_waits;
#endif
            asm("nop");
        }

#ifdef TRIQUAD_STATS
        if (!data()) {
              _data_waits = prev_data_waits;     // separate waits from timeouts
            ++_data_timeouts               ;
        }
#endif
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
#endif  // #if TRIQUAD_DATA_WAIT_US > 0




} // namespace quad4me
