#include <mcu.hxx>
#ifdef RANDOM_DELAY_US
#include <mrt.hxx>
#endif
#include <sct.hxx>


#ifdef TRIQUAD_TRI2B
#include <tri2b.hxx>
#ifdef TRIQUAD_INTERRUPTS
#include <tri2b.inl>
#endif
#include <tri2b_config.hxx>

typedef triquad::Tri2bBase    TriquadBase;
typedef triquad::Tri2b        Triquad    ;

namespace triquad_config = tri2b_config;
#endif  // #ifdef TRIQUAD_TRI2B


#ifdef TRIQUAD_QUAD4ME
#include <quad4me.hxx>
#ifdef TRIQUAD_INTERRUPTS
#include <quad4me.inl>
#endif
#include <quad4me_config.hxx>

typedef triquad::Quad4meBase    TriquadBase;
typedef triquad::Quad4me        Triquad    ;

namespace triquad_config = quad4me_config;
#endif  // #ifdef TRIQUAD_QUAD4ME



// needed for GDB access from breakpoint anywhere
volatile LPC_GPIO_PORT_TypeDef  *debug_gpio_port = LPC_GPIO_PORT;



namespace randomtest {

// hardcoded, [0...32] bits of data, inclusive (0x20 max)
static const uint8_t        META_LEN = 6                          ,
                            META_MSK = ((1 << (META_LEN - 1)) - 1);

static const char* const    MCU_STRING __attribute__((used)) = "LPC824";

static const uint8_t        SCT_PRESCALE = 250,  // maximum "even" value
                            SCT_KHZ      =   mcu_config::MAIN_CLOCK_KHZ
                                           / SCT_PRESCALE;
static const uint32_t       TIMER_MAX    = 0xffffffff;  // maximum timeout

lpc::SCT                    statistics_timer(SCT_PRESCALE);

Triquad                     triquad_random(TRIQUAD_NUM_NODES,
                                           TRIQUAD_ARBT_BITS,
                                           META_LEN         ,
                                           TRIQUAD_ID        );



#if RANDOM_DELAY_US > 0
static const uint8_t    RANDOM_DELAY_CHANNEL = 0;
lpc::MRT                random_delay_timer;

static_assert(RANDOM_DELAY_CHANNEL != triquad_config::SYNC_NODES_MRT_CHANNEL,
	     "RANDOM_DELAY_CHANNEL != triquad_config::SYNC_NODES_MRT_CHANNEL");

uint32_t milliseconds_to_clocks(
const uint32_t  milliseconds)
{
    return baresil::lpc8xx::mcu::milliseconds_to_clocks(milliseconds);
}

uint32_t microseconds_to_clocks(
const uint32_t  microseconds)
{
    return baresil::lpc8xx::mcu::microseconds_to_clocks(microseconds);
}


void random_delay_timer_one_shot(
uint32_t    delay)
{
    random_delay_timer.one_shot(RANDOM_DELAY_CHANNEL, delay);
}

bool random_delay_timer_is_running()
{
    return random_delay_timer.is_running(RANDOM_DELAY_CHANNEL);
}
#endif  // #if RANDOM_DELAY_US > 0



void randomtest_init() {
    baresil::lpc8xx::mcu::init();
}



void post_reset() {
}

} // namespace randomtest
