#ifndef TIM_HXX
#define TIM_HXX

#include <stdint.h>

#include <stm32f103xb.h>   // because can't forward-declare typedefs


namespace stm32f10_12357_xx {

class TIM {
  public:
    static const uint16_t   MAX_COUNTS = 0xffff;

    constexpr
    TIM()
    :   _timer(0)
    {}

    void        init(TIM_TypeDef    *timer,
                     uint16_t        prescaler);

    // prescaler is clock_division-1, i.e. 0 is divide by 1
    uint16_t    prescale(                    ) const { return _timer->PSC; }
    void        prescale(uint16_t   prescaler);

    void        one_shot(uint16_t   counts,
                         bool       interrupt = false);

    void        clear_interrupts();

    void        busy_delay(uint16_t counts);

    bool        is_running() const;

    uint16_t    count() const;

    void        pause (),
                resume();


  protected:
    TIM_TypeDef     *_timer;

};

} // namespace stm32f10_12357_xx

#endif   // ifndef TIM_HXX
