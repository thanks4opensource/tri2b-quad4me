#ifndef SCT_HXX
#define SCT_HXX

#include <stdint.h>

namespace lpc {

class SCT {
  public:
    static const bool   ENABLE_INTERRUPT = true,
                        NO_INTERRUPT     = false;

    constexpr SCT(uint8_t   prescale) : _prescale(prescale) {}

    void            init     (uint8_t   prescale)       { _prescale = prescale;}

    uint32_t        prescaler(                  ) const { return _prescale    ;}
    void            prescaler(uint8_t   prescale)       { _prescale = prescale;}

    void        one_shot(uint32_t   counts,
                             bool       interrupt = NO_INTERRUPT);

    void        busy_delay(uint32_t counts,
                               bool     interrupt = NO_INTERRUPT);

    uint32_t    count();

    bool        is_running();

    void        pause (),
                resume();

  protected:
    uint32_t    _prescale;

};

} // namespace lpc

#endif   // ifndef SCT_HXX
