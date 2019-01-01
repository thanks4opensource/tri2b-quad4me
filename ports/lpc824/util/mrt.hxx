#ifndef MRT_HXX
#define MRT_HXX

#include <stdint.h>

namespace lpc {

class MRT {
  public:
    static const uint32_t   MAX_COUNTS       = 0x7fffffff;  // bits 30..0
    static const bool       ENABLE_INTERRUPT = true,
                            NO_INTERRUPT     = false;


    static void     init() {}

    static void     one_shot(uint8_t    channel,
                             uint32_t   counts,
                             bool       interrupt = NO_INTERRUPT);

    static void     busy_delay(uint8_t  channel,
                               uint32_t counts,
                               bool     interrupt = NO_INTERRUPT);

    static bool is_running(uint8_t      channel);

    static uint8_t  get_idle_channel();

    // http://blog.nano-age.co.uk/2014/08/some-other-lpc810-mrt-uses.html
    static const uint32_t   OVERHEAD_CLOCKS = 0;   // subtract 286
};

} // namespace lpc

#endif   // ifndef MRT_HXX
