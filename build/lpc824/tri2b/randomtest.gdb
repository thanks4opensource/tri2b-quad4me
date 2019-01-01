define tri2b
    printf "_ARBT_BITS        : %-2d\n", randomtest::triquad_random._ARBT_BITS
    printf "_META_BITS        : %-2d\n", randomtest::triquad_random._META_BITS

    printf "_NODE_ID          : %-2d\n", randomtest::triquad_random._NODE_ID
    printf "_rank             : %-2d\n", randomtest::triquad_random._rank

    echo _state            : \ 
    output randomtest::triquad_random._state
    echo \n

    echo _phase            : \ 
    output randomtest::triquad_random._phase
    echo \n

    echo _role             : \ 
    output randomtest::triquad_random._role
    echo \n

    printf "_sendbuf          : 0x%x\n", randomtest::triquad_random._sendbuf
    printf "_recvbuf          : 0x%x\n", randomtest::triquad_random._recvbuf
    printf "_arbtbit          : %d\n"  , randomtest::triquad_random._arbtbit
    printf "_arbt             : %d\n"  , randomtest::triquad_random._arbt
    printf "_meta             : %d\n"  , randomtest::triquad_random._meta
    printf "_bit              : 0x%x 0b", randomtest::triquad_random._bit
    output/t randomtest::triquad_random._bit
    echo \n

    printf "_pendmet          : %d\n"    , randomtest::triquad_random._pendmet
    printf "_pendbuf          : 0x%08x\n", randomtest::triquad_random._pendbuf

    echo _pending          : \ 
    output randomtest::triquad_random._pending
    echo \n

    printf "_waits            : %d\n", randomtest::triquad_random._waits
    printf "_data_waits       : %d\n", randomtest::triquad_random._data_waits
    printf "_data_timeouts    : %d\n", randomtest::triquad_random._data_timeouts

    printf "data              : %d -> %d\n", \
           (bool)(LPC_GPIO_PORT->SET0 & tri2b_config::DATA_GPIO_BIT), \
           (bool)(LPC_GPIO_PORT->PIN0 & tri2b_config::DATA_GPIO_BIT)
    printf "alrt              : %d -> %d\n", \
           (bool)(LPC_GPIO_PORT->SET0 & tri2b_config::ALRT_GPIO_BIT), \
           (bool)(LPC_GPIO_PORT->PIN0 & tri2b_config::ALRT_GPIO_BIT)
    printf "ltch              : %d -> %d\n", \
           (bool)(LPC_GPIO_PORT->SET0 & tri2b_config::LTCH_GPIO_BIT), \
           (bool)(LPC_GPIO_PORT->PIN0 & tri2b_config::LTCH_GPIO_BIT)

    printf "alrt_rise         : 0x%08x %c\n"				, \
	   LPC_PIN_INT->RISE						, \
	   LPC_PIN_INT->RISE & tri2b_config::ALRT_PININT_BIT ? 'A' : '.'
    printf "ltch_rise         : 0x%08x %c\n"				, \
	   LPC_PIN_INT->RISE						, \
	   LPC_PIN_INT->RISE & tri2b_config::LTCH_PININT_BIT ? 'L' : '.'
end     




dprintf randomtest::check_message:check, \
"%s: %d|%d  %d|%d  %s-%s  %c|%08x|%08x  %2d|0x%-8x  %3d,%-3d %3d  %2d|0x%-8x\n", \
s_or_r, \
randomtest::triquad_random._NODE_ID, \
randomtest::triquad_random._rank, \
rank_ids._rank_to_id[arbt], \
arbt, \
\
randomtest::triquad_random._PHASE_STRINGS[randomtest::triquad_random._phase], \
randomtest::triquad_random. _SCOR_STRINGS[randomtest::triquad_random._scor ], \
\
LPC_MRT->Channel[DELAY_CHANNEL].STAT & mrt::status::IS_RUNNING_BIT ? 'r' : '.', \
LPC_MRT->Channel[DELAY_CHANNEL].TIMER, \
LPC_MRT->Channel[DELAY_CHANNEL].INTVAL, \
meta, \
data, \
num_sends, \
num_recvs, \
num_mesgs, \
pend_meta, \
pend_data

# dprintf main:send, \
# "send: %d|%d  -|-  %06x|%08x  %2d|0x%-8x  %3d,%-3d %3d\n", \
# randomtest::triquad_random._NODE_ID, \
# randomtest::triquad_random._rank, \
# random_delay & delay_mask, \
# LPC_MRT->Channel[DELAY_CHANNEL].TIMER, \
# pend_meta, \
# pend_data, \
# num_sends, \
# num_recvs, \
# num_mesgs



define results
    printf "NUM_NODES    : %d\n",   TRIQUAD_NUM_NODES
    printf "ARBT_LEN     : %d\n",   TRIQUAD_ARBT_BITS
    printf "META_LEN     : %d\n",   randomtest::META_LEN
    printf "_NODE_ID     : %d\n",   randomtest::triquad_random._NODE_ID
    printf "_NUM_NODES   : %d\n",   randomtest::triquad_random._NUM_NODES
    printf "_ARBT_BITS   : %d\n",   randomtest::triquad_random._ARBT_BITS
    printf "_META_BITS   : %d\n",   randomtest::triquad_random._META_BITS
    printf "_NODE_ID     : %d\n",   randomtest::triquad_random._NODE_ID
    echo mcu          : \ 
    output randomtest::MCU_STRING
    echo \n
    echo OPTIMIZE     : \ 
    output TRIQUAD_OPTIMIZE_STRING
    echo \n
    echo t2b/q4m      : \ 
    output randomtest::T2B_QQ4M_STRING
    echo \n
    echo intrpt/poll  : \ 
    output randomtest::INTRPT_POLL_STRING
    echo \n
    echo protocol     : \ 
    output randomtest::PROTOCOL_STRING
    echo \n
    echo dynamic rank : \ 
    output randomtest::DYN_RNK_STRING
    echo \n
    printf "MIN_DYN_RANK : %d\n",   MIN_DYNAMIC_RANK
    printf "MAX_DYN_RANK : %d\n",   MAX_DYNAMIC_RANK
    echo glitch_filt  : \ 
    output randomtest::NXP_GLITCH_FILTER_STRING
    echo \n
    echo progress     : \ 
    output randomtest::PROGRESS_STRING
    echo \n
    echo stats        : \ 
    output randomtest::STATS_STRING
    echo \n
    printf "DATA_WAIT_US : %d\n",   TRIQUAD_DATA_WAIT_US
    printf "MIN_HIGH_US  : %d\n",   TRIQUAD_MIN_HIGH_US
    printf "RAND_DLY_US  : %d\n",   randomtest::RANDOM_DELAY_K
    printf "delay_mask   : 0x%x\n", randomtest::delay_mask
    echo mcu          : \ 
    output randomtest::MCU_STRING
    echo \n
    printf "CLOCK_DIV    : %d\n",   mcu_config::MAIN_CLOCK_DIVISOR
    printf "CLOCK_KHZ    : %d\n",   mcu_config::MAIN_CLOCK_KHZ
    printf "CLOCK_MHZ    : %d\n",   mcu_config::MAIN_CLOCK_MHZ
    printf "SCT_PRESCALE : %d\n",   randomtest::SCT_PRESCALE
    printf "SCT_KHZ      : %d\n",   randomtest::SCT_KHZ
    printf "REPORT_AT    : %d\n",   randomtest::REPORT_AT_K + 1
    printf "RESET_AT     : %d\n",   randomtest::RESET_AT_K  + 1
    printf "BUSY_MASK    : 0x%x\n", randomtest::BUSY_MASK_K
    printf "reported_at  : %d\n",   randomtest::reported_at
    # printf "resetted_at  : %d\n",   randomtest::resetted_at
    echo dirs         : \ 
    output/t randomtest::dirs
    echo \n
    echo mesgs        : \ 
    output randomtest::mesgs
    echo \n
    printf "num_bits     : %d\n",   randomtest::num_bits
    printf "num_bytes    : %d\n",   randomtest::num_bits >> 3
    printf "num_mesgs    : %d\n",   randomtest::num_mesgs
    printf "num_redys    : %d\n",   randomtest::num_redys
    printf "num_pends    : %d\n",   randomtest::num_pends
    printf "num_timrs    : %d\n",   randomtest::num_timrs
    printf "pend_only    : %d\n",   randomtest::pend_only
    printf "timr_only    : %d\n",   randomtest::timr_only
    printf "num_sends    : %d\n",   randomtest::num_sends
    printf "num_recvs    : %d\n",   randomtest::num_recvs
    printf "num_wnnrs    : %d\n",   randomtest::num_wnnrs
    printf "last_recv    : %d\n",   randomtest::last_recv
    printf "num_nzwnr    : %d\n",   randomtest::num_nzwnr
    printf "num_losrs    : %d\n",   randomtest::num_losrs
    printf "num_rcvrs    : %d\n",   randomtest::num_rcvrs
    printf "last_pend    : %d\n",   randomtest::last_pend
    printf "last_send    : %d\n",   randomtest::last_send
    printf "num_loops    : %d\n",   randomtest::num_loops
    printf "_waits       : %d\n",   randomtest::triquad_random._waits
    printf "_data_waits  : %d\n",   randomtest::triquad_random._data_waits
    printf "_data_timout : %d\n",   randomtest::triquad_random._data_timeouts
    printf "_waits/bit   : %.3f\n",   (float)randomtest::triquad_random._waits \
				    / (float)randomtest::num_bits
    printf "_datwait/bit : %.3f\n",   (float)randomtest::triquad_random._data_waits \
				    / (float)randomtest::num_bits
    printf "_datmout/bit : %.3f\n",   (float)randomtest::triquad_random._data_timeouts \
				    / (float)randomtest::num_bits
    printf "sct_count    : %d\n",   randomtest::statistics_timer.count()
    set var $milliseconds =   (float)randomtest::statistics_timer.count() \
                            / (float)randomtest::SCT_KHZ
    set var $metdat_bits   =   randomtest::num_bits                         \
                            +    randomtest::num_mesgs                      \
                               * randomtest::triquad_random._META_BITS 
    set var $total_bits   =   $metdat_bits                                  \
                            +    randomtest::num_mesgs                      \
                               * randomtest::triquad_random._ARBT_BITS
    printf "milliseconds : %.1f\n",   $milliseconds
    if $milliseconds == 0.0
        set var $milliseconds = 1
    end
    if $total_bits == 0
        set var $total_bits = 1
    end
    printf "bits/sec     : %.1f\n", 1000.0 * (float)$metdat_bits / $milliseconds
    printf "bytes/sec    : %.1f\n",  125.0 * (float)$metdat_bits / $milliseconds
    printf "clocks/bit   : %.1f\n",   (float)randomtest::SCT_PRESCALE        \
                        * (  (float)randomtest::statistics_timer.count()     \
                           / (float)$total_bits                         )
end

def damd
    printf "id        : %d\n", randomtest::triquad_random._NODE_ID
    printf "_rank     : %d\n", randomtest::triquad_random._rank
    printf "num_mesgs : %d\n", randomtest::num_mesgs
    printf "current   : %d\n", randomtest::num_mesgs & randomtest::HISTORY_MSK
    echo msgnm     : \ 
    output randomtest::msgnm
    echo \n
    echo dirs      : \ 
    output/t randomtest::dirs
    echo \n
    echo arbts     : \ 
    output/d randomtest::arbts
    echo \n
    echo sndrs     : \ 
    output/d randomtest::sndrs
    echo \n
    echo metas     : \ 
    output/d randomtest::metas
    echo \n
    echo datas     : \ 
    output/x randomtest::datas
    echo \n
end



break randomtest::check_message:results
commands 2
    results
    shell date
    continue
end



define date_continue
    shell date
    continue
end



# dprintf tri2b::Tri2bBase::protocol, \

# GDB coredumps if attempt to set breakpoint at (this particular) label
# dprintf tri2b_base.cxx:loop

dprintf tri2b_base.cxx:162, \
"%d%d %s-%s-%s %c%c%c->%c%c%c %c%c %08x-%08x-%08x %x:%d:%-2d %2d/%c/0x%08x\n", \
_NODE_ID, \
_rank, \
_STATE_STRINGS[_state], \
_PHASE_STRINGS[_phase], \
_ROLE_STRINGS [_role ], \
\
LPC_GPIO_PORT->SET0 & tri2b_config::DATA_GPIO_BIT ? 'd' : '.', \
LPC_GPIO_PORT->SET0 & tri2b_config::ALRT_GPIO_BIT ? 'a' : '.', \
LPC_GPIO_PORT->SET0 & tri2b_config::LTCH_GPIO_BIT ? 'l' : '.', \
LPC_GPIO_PORT->PIN0 & tri2b_config::DATA_GPIO_BIT ? 'd' : '.', \
LPC_GPIO_PORT->PIN0 & tri2b_config::ALRT_GPIO_BIT ? 'a' : '.', \
LPC_GPIO_PORT->PIN0 & tri2b_config::LTCH_GPIO_BIT ? 'l' : '.', \
\
LPC_PIN_INT->RISE & tri2b_config::ALRT_PININT_BIT ? 'A' : '.', \
LPC_PIN_INT->RISE & tri2b_config::LTCH_PININT_BIT ? 'L' : '.', \
\
_sendbuf, \
_recvbuf, \
_bit    , \
\
_arbt   , \
_arbtbit, \
_meta   , \
\
_pendmet, \
_pending ? 'p' : '.', \
_pendbuf

disable
