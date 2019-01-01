#include <mcu.hxx>

#include <tim.hxx>
#include <tim_m_s.hxx>


#ifdef TRIQUAD_TRI2B
#include <tri2b.hxx>
#ifdef TRIQUAD_INTERRUPTS
#include <tri2b.inl>
#endif
#include <tri2b_config.hxx>

typedef tri2b::Tri2bBase        TriquadBase;
typedef tri2b::Tri2b            Triquad    ;

namespace   triquad_config = tri2b_config;

using namespace tri2b;  // for randomtest.cxx
#endif  // #ifdef TRIQUAD_TRI2B


#ifdef TRIQUAD_QUAD4ME
#include <quad4me.hxx>
#ifdef TRIQUAD_INTERRUPTS
#include <quad4me.inl>
#endif
#include <quad4me_config.hxx>

typedef quad4me::Quad4meBase    TriquadBase;
typedef quad4me::Quad4me        Triquad    ;

namespace   triquad_config = quad4me_config;

using namespace quad4me;  // for randomtest.cxx
#endif  // #ifdef TRIQUAD_QUAD4ME




namespace randomtest {

// hardcoded, [0...32] bits of data, inclusive (0x20 max)
static const uint8_t        META_LEN = 6                          ,
                            META_MSK = ((1 << (META_LEN - 1)) - 1);

static const char* const      MCU_STRING __attribute__((used))
                            = "STM32F10_12357_XX";

static const uint16_t       TIM_PRESCALE = static_cast<uint16_t>(
                                              baresil
                                            ::stm32f10_12357_xx
                                            ::mcu
                                            ::milliseconds_to_clocks(1)),
                            TIM_KHZ =   mcu_config::MAIN_CLOCK_KHZ
                                        / TIM_PRESCALE                  ;

static const uint16_t       TIMER_MAX = stm32f10_12357_xx::TimMS::MAX_COUNTS;

stm32f10_12357_xx::TimMS    statistics_timer;

Triquad                     triquad_random(TRIQUAD_NUM_NODES,
                                           TRIQUAD_ARBT_BITS,
                                           META_LEN         ,
                                           TRIQUAD_ID        );


#if RANDOM_DELAY_US > 0
stm32f10_12357_xx::TIM  random_delay_timer;

uint32_t milliseconds_to_clocks(
const uint32_t  milliseconds)
{
    return baresil::stm32f10_12357_xx::mcu::milliseconds_to_clocks(milliseconds);
}

uint32_t microseconds_to_clocks(
const uint32_t  microseconds)
{
    return baresil::stm32f10_12357_xx::mcu::microseconds_to_clocks(microseconds);
}


void random_delay_timer_one_shot(
uint32_t    delay)
{
    random_delay_timer.one_shot(delay);
}

bool random_delay_timer_is_running()
{
    return random_delay_timer.is_running();
}
#endif  // #if RANDOM_DELAY_US > 0



void randomtest_init()
{
    baresil::stm32f10_12357_xx::mcu::init();

    statistics_timer.init(triquad_config::STATISTICS_TIMER_MASTR    ,
                          triquad_config::STATISTICS_TIMER_SLAVE    ,
                          triquad_config::STATISTICS_TIMER_MSTR_SLAV,
                          TIM_PRESCALE - 1);  // -1 due to timer API

#if RANDOM_DELAY_US > 0
    // will actually be changed to milliseconds by reset_delay_start(),
    // but then back to microseconds in post_reset()
    //
    // needed because reusing  TIM4 for both reset_nodes_timer and
    // random_delay_timer
    random_delay_timer.init(triquad_config::RANDOM_DELAY_TIMER,
                            static_cast<uint16_t>(  baresil
                                                  ::stm32f10_12357_xx
                                                  ::mcu
                                                  ::microseconds_to_clocks(1)));
#endif
}

} // namespace randomtest
