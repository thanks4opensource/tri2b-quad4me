#ifndef SYS_TICK_HXX
#define SYS_TICK_HXX

#include <stdint.h>

#ifndef __CORTEX_M
#error #include core_cmNxxx.h before __FILE__
#endif


#include <bitops.hxx>



namespace arm {

static SysTick_Type* const SYS_TICK_PTR = SysTick;
#undef SysTick


class SysTick {
  public:
    static const uint32_t       MAX_COUNTS = 0xffffff;

    SysTick() {}

    static void one_shot(
    uint32_t    counts             ,
    bool        normal_clock = true,
    bool        interrupt    = false)
    {
        SYS_TICK_PTR->CTRL = 0;  // halt
        SYS_TICK_PTR->VAL  = 0;  // ensure will start from LOAD value

        SYS_TICK_PTR->LOAD = counts & MAX_COUNTS;

          SYS_TICK_PTR->CTRL
        =   static_cast<unsigned>(normal_clock) << SysTick_CTRL_CLKSOURCE_Pos
          | static_cast<unsigned>(interrupt   ) << SysTick_CTRL_TICKINT_Pos
          | SysTick_CTRL_ENABLE_Msk                                          ;
    }

    static void busy_delay(
    uint32_t    counts             ,
    bool        normal_clock = true,
    bool        interrupt    = false)
    {
        one_shot(counts, normal_clock, false);
        while (!is_running())
            asm("nop");
    }

    static bool is_running()
    {
        return bitops::GET_BITS(SYS_TICK_PTR->CTRL, SysTick_CTRL_COUNTFLAG_Msk);
    }

    static uint32_t count()
    {
        return SYS_TICK_PTR->VAL;
    }

    static void pause()
    {
        bitops::CLR_BITS(SYS_TICK_PTR->CTRL, SysTick_CTRL_ENABLE_Msk);
    }

    static void resume()
    {
        bitops::SET_BITS(SYS_TICK_PTR->CTRL, SysTick_CTRL_ENABLE_Msk);
    }


    // undefined results if counter has underflowed more than once
    // begin = count();
    static uint32_t elapsed(
    uint32_t    begin)
    {
        uint32_t    counter = count();

        return   counter < begin
               ? begin - counter
               : (begin | (MAX_COUNTS + 1)) - counter;
    }

};

} // namespace arm

#endif   // ifndef SYS_TICK_HXX
