#include <stm32f10_12357_xx.hxx>

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <tim.hxx>


using namespace baresil::stm32f10_12357_xx;


namespace stm32f10_12357_xx {

void TIM::init(
TIM_TypeDef     *timer    ,
uint16_t         prescaler)

{
    _timer = timer;

    prescale(prescaler);
}



void TIM::prescale(
uint16_t    prescaler)
{
    _timer->PSC = prescaler;

    // impossible TIMx bugs: first one_shot() is zero delay,
    // so do fake one here
    // can't do with PSC==0 and ARR==1, subsequent change of PSC makes
    //   first real one fail again
    busy_delay(2);   // can't be 0 or 1
}



void TIM::one_shot(
uint16_t    counts,
bool        interrupt)
{
    // stop timer before configuring
    _timer->CR1 = 0;

    // clear "finished" flag (from previous invocation)
    _timer->SR = 0;

    if (interrupt) {
        _timer->SR   =  0;            // not using other bits, clear interrupt(s)
        _timer->DIER = TIM_DIER_UIE;  // not using other bits, all 0 default
    }

    _timer->ARR = counts;
    _timer->CNT = 0;
    _timer->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;
}



void TIM::clear_interrupts()
{
    _timer->SR = 0;
}



void TIM::busy_delay(
uint16_t    counts)
{
    one_shot(counts, false);

    while (is_running())
        delay: asm("nop");
}


uint16_t TIM::count()
const
{
    return _timer->CNT;
}



bool TIM::is_running()
const
{
    return !bitops::GET_BITS(_timer->SR, TIM_SR_UIF);
}



void TIM::pause()
{
    bitops::CLR_BITS(_timer->CR1, TIM_CR1_CEN);
}



void TIM::resume()
{
    bitops::SET_BITS(_timer->CR1, TIM_CR1_CEN);
}

} // namespace stm32f10_12357_xx
