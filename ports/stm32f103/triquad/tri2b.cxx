#include <stm32f103xb.h>
#include <core_cm3.h>   // must be after stm32f103x6.h for NVIC_type

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <mcu.hxx>
#include <tim.hxx>

#if TRIQUAD_MIN_HIGH_US > 0 || TRIQUAD_DATA_WAIT_US > 0
#include <sys_tick.hxx>
#endif

#include <tri2b_config.hxx>

#include "tri2b.hxx"
#include "tri2b.inl"



namespace {
    stm32f10_12357_xx::TIM      reset_timer;
}

using namespace baresil::stm32f10_12357_xx;



namespace tri2b {

// Tri2b derived class-specific methods
//

void Tri2b::init()
{
    Tri2bBase::init();

    // alrt, ltch and data all low here (were high at GDB start)
    // must do before setting GPIO to output, otherwise are temporarilly
    // all low, breaks other nodes waiting for all high then ltch low
    set_data();
    set_ltch();
    set_alrt();

    // set pins to GPIO output, 2MHz max
    bitops::CHG_BITS(*tri2b_config::GPIO_CR                               ,

                       (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << tri2b_config::ALRT_GPIO_CRLH_POS              )
                     | (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << tri2b_config::LTCH_GPIO_CRLH_POS              )
                     | (   (gpio::crlh::MODE_MASK | gpio::crlh::CONF_MASK)
                        << tri2b_config::DATA_GPIO_CRLH_POS              ),

                       (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << tri2b_config::ALRT_GPIO_CRLH_POS              )
                     | (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << tri2b_config::LTCH_GPIO_CRLH_POS              )
                     | (   (  gpio::crlh::MODE_OUTPUT_2MHZ
                            | gpio::crlh::CONF_OUTPUT_OPEN_DRAIN         )
                        << tri2b_config::DATA_GPIO_CRLH_POS              ));

    // set up interrupts / edge detection
    //

    // mapping of pins to interrupts (RCC_APB2ENR_AFIOEN needs to be
    //   permanently enabled in mcu::init() -- can't enable here,
    //   set AFIO->EXTICR, then disable
    bitops::CHG_BITS(AFIO->EXTICR[tri2b_config::ALRT_LTCH_AFIO_EXTICR_NDX],
                       tri2b_config::ALRT_AFIO_EXTICR_MASK
                     | tri2b_config::LTCH_AFIO_EXTICR_MASK                ,
                       tri2b_config::ALRT_AFIO_EXTICR_PORT
                     | tri2b_config::LTCH_AFIO_EXTICR_PORT                );

#if 1  // have to do this to get edge detection
    // enable rising and falling edge trigger
    // set bit to 1 to not mask
    bitops::SET_BITS(EXTI->IMR,
                       tri2b_config::ALRT_EXTI_BIT
                     | tri2b_config::LTCH_EXTI_BIT);
#endif

#ifdef TRIQUAD_INTERRUPTS
    // set up interrupts / edge detection
    //

    // mapping of pins to interrupts (RCC_APB2ENR_AFIOEN needs to be
    //   permanently enabled in mcu::init() -- can't enable here,
    //   set AFIO->EXTICR, then disable
    bitops::CHG_BITS(AFIO->EXTICR[tri2b_config::ALRT_LTCH_AFIO_EXTICR_NDX],
                     tri2b_config::ALRT_AFIO_EXTICR_MASK                    ,
                     tri2b_config::ALRT_AFIO_EXTICR_PORT                     );
#endif

    reset_timer.init(tri2b_config::RESET_NODES_TIMER,
                     static_cast<uint16_t>(  baresil
                                           ::stm32f10_12357_xx
                                           ::mcu
                                           ::milliseconds_to_clocks(1)));

#if defined(TRIQUAD_STATS) || TRIQUAD_MIN_HIGH_US > 0 || TRIQUAD_DATA_WAIT_US > 0
    // not really one shot, will run continuously
    arm::SysTick::one_shot(arm::SysTick::MAX_COUNTS);
#endif

    enbl_alrt_rise();
    enbl_ltch_rise();
}



void Tri2b::enbl_alrt_rise()
{
    bitops::CLR_BITS(EXTI->PR,   tri2b_config::ALRT_EXTI_BIT);
    bitops::SET_BITS(EXTI->RTSR, tri2b_config::ALRT_EXTI_BIT);
}

void Tri2b::enbl_ltch_rise()
{
    bitops::CLR_BITS(EXTI->PR,   tri2b_config::LTCH_EXTI_BIT);
    bitops::SET_BITS(EXTI->RTSR, tri2b_config::LTCH_EXTI_BIT);
}



// Tri2bBase architecture-specific implemented methods
//

void Tri2bBase::post_reset()
{
    if (tri2b_config::RANDOM_DELAY_TIMER == tri2b_config::RESET_NODES_TIMER)
        // reset timebase because reusing same timer

        reset_timer.init(tri2b_config::RANDOM_DELAY_TIMER,
                         static_cast<uint16_t>(  baresil
                                               ::stm32f10_12357_xx
                                               ::mcu
                                               ::microseconds_to_clocks(1)));
}



void Tri2bBase::reset_delay_start()
{
    reset_timer.one_shot(tri2b_config::SYNC_NODES_DELAY_MILLISECS);
}


bool Tri2bBase::reset_delay_wait()
{
    return reset_timer.is_running();
}



#ifdef TRIQUAD_INTERRUPTS
void Tri2bBase::enable_interrupt() {
    // clear any pending edge detextion
    bitops::CLR_BITS(EXTI->PR,   tri2b_config::ALRT_EXTI_BIT);

    // enable rising and falling edge trigger interrupt
    // set bit to 1 to not mask interrupt
    bitops::SET_BITS(EXTI->IMR, tri2b_config::ALRT_EXTI_BIT);

    // clear any pending interrupt
    NVIC->ICPR[0] = tri2b_config::ALRT_NVIC_BIT;

    // enable interrupt
    NVIC->ISER[0] = tri2b_config::ALRT_NVIC_BIT;
}
#endif   // ifdef TRIQUAD_INTERRUPTS



#if TRIQUAD_DATA_WAIT_US > 0
void Tri2bBase::set_data() {
    tri2b_config::GPIO->BSRR = tri2b_config::DATA_GPIO_BIT;

    uint32_t    systick_start = arm::SysTick::count();

    if (_phase == Phase::IDLE || _phase == Phase::ARBT) {
        // need timout: other node(s) might be pulling line down
        while (   !data()
               &&   arm::SysTick::elapsed(systick_start)
                  < baresil::stm32f10_12357_xx::mcu::microseconds_to_clocks(
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
