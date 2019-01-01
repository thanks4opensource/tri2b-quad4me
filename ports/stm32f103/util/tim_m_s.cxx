#include <stm32f10_12357_xx.hxx>

#include <bitops.hxx>
#include <stm32f10_12357_xx.hxx>

#include <tim_m_s.hxx>


using namespace baresil::stm32f10_12357_xx;


namespace stm32f10_12357_xx {

void TimMS::init(
TIM_TypeDef     *mastr       ,
TIM_TypeDef     *slave       ,
uint32_t         master_slave,
uint16_t         prescaler    )
{
    _mastr = mastr;
    _slave = slave;

    prescale(master_slave, prescaler);
}



void TimMS::prescale(
uint32_t    master_slave,
uint16_t    prescaler    )
{
    // stop timers before configuring
    _mastr->CR1 = 0;
    _slave->CR1 = 0;

    // configure master
    _mastr->CR2 = TIM_CR2_MMS_1;  // output on update event
    _mastr->SR  = 0            ;  // clear
    _mastr->PSC = prescaler    ;

    // configure slave
    _slave->SR   = 0           ;  // clear
    _slave->PSC  = 0           ;  // no prescaling (divide by 1)
    _slave->SMCR =   master_slave            // TS bits
                   | tim_2_5::smcr::SMS_EXTERNAL;    // triggered by master
                                                     // update event

    // impossible TIMx bugs: first time is zero delay,
    // so do fake one here
    // can't do with PSC==0 and ARR==1, subsequent change of PSC makes
    //   first real one fail again
    busy_delay(1, 2);
}



void TimMS::one_shot(
uint16_t    slave_counts,
uint16_t    mastr_counts,
bool        interrupt   )
{
    // stop timers before configuring
    _mastr->CR1 = 0;
    _slave->CR1 = 0;

    // clear "finished" flag (from previous invocation)
    _mastr->SR = 0;
    _slave->SR = 0;

    if (interrupt)
        _slave->DIER = TIM_DIER_UIE;  // not using other bits, all 0 default

    _slave->ARR = slave_counts;
    _mastr->ARR = mastr_counts;

    _slave->CNT = 0;
    _mastr->CNT = 0;

    // start
    _slave->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;
    _mastr->CR1 =               TIM_CR1_CEN;
}



void TimMS::clear_interrupts()
{
    _slave->SR = 0;
}



void TimMS::busy_delay(
uint16_t    slave_counts,
uint16_t    mastr_counts)
{
    one_shot(slave_counts, mastr_counts, false);

    while (is_running())
        delay: asm("nop");
}


uint32_t TimMS::count()
const
{
    volatile uint16_t   mastr_count,
                        slave_count;

    // handle non-atomic read of two registers, overflow of mastr during reads
    do {
        mastr_count = _mastr->CNT;
        slave_count = _slave->CNT;
    } while (mastr_count != _mastr->CNT);

    return (slave_count << 16) | mastr_count;
}



bool TimMS::is_running()
const
{
    return !bitops::GET_BITS(_slave->SR, TIM_SR_UIF);
}



void TimMS::pause()
{
    bitops::CLR_BITS(_mastr->CR1, TIM_CR1_CEN);
    bitops::CLR_BITS(_slave->CR1, TIM_CR1_CEN);
}



void TimMS::resume()
{
    bitops::SET_BITS(_mastr->CR1, TIM_CR1_CEN);
    bitops::SET_BITS(_slave->CR1, TIM_CR1_CEN);
}

} // namespace stm32f10_12357_xx
