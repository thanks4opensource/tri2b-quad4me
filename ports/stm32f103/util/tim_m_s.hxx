#ifndef TIM_M_S_HXX
#define TIM_M_S_HXX

#include <stdint.h>

#include <stm32f103xb.h>   // because can't forward-declare typedefs


namespace stm32f10_12357_xx {

class TimMS {
  public:
    static const uint16_t   MAX_COUNTS = 0xffff;

    constexpr
    TimMS()
    :   _mastr(0),
        _slave(0)
    {}

    void        init(TIM_TypeDef        *mastr       ,
                     TIM_TypeDef        *slave       ,
                     uint32_t            master_slave,
                     uint16_t            prescaler    );

    // prescaler is clock_division-1, i.e. 0 is divide by 1
    uint16_t    prescale() const { return _mastr->PSC;}

    void        prescale(uint32_t   master_slave,
                         uint16_t   prescaler   );

    void        one_shot(uint16_t   slave_counts             ,
                         uint16_t   mastr_counts = MAX_COUNTS,
                         bool       interrupt    = false     );

    void        clear_interrupts();

    void        busy_delay(uint16_t slave_counts             ,
                           uint16_t mastr_counts = MAX_COUNTS);

    bool        is_running() const;

    uint32_t    count() const;

    void        pause (),
                resume();


  protected:
    TIM_TypeDef     *_mastr,
                    *_slave;

};

} // namespace stm32f10_12357_xx

#endif   // ifndef TIM_M_S_HXX
