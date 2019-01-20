/*

Functional test for "tri2b" and "quad4me" serial communications
protocol libaries

See file "README.md" for protocol documentation.

Sends messages of random length and content and receives same from
other nodes. Uses pseudo random number generator (xor_shift
algorithm) and thus knows sequence of messages (length+content)
expected from other nodes.


Many build options (some mandatory) incuding:

TRIQUAD_POLLING or TRIQUAD_INTERRUPTS
Polled or interrupt driven invocation of protocol library object's
protocol() method. See README.md for discussion of pros and cons of
each in actual client.

TRIQUAD_TRI2B or TRIQUAD_TRI2B
Choice of protocol.

TRIQUAD_PROGRESS
Optionally use with TRIQUAD_POLLING for minor short-circuiting of
main loop if protocol not waiting. See README.md for discussion of
possible usefulness in actual client.

TRIQUAD_BIT_BY_BIT or TRIQUAD_WHOLE_MESSAGE
Protocol implementation. If BIT_BY_BIT, protocol library object's
"protocol()" method returns when waiting for and/or receiving each
bit of message. If WHOLE_MESSAGE, returns only on message completion.
See README.md for discussion of pros and cons of each in actual client


TRIQUAD_NUM_NODES    number of nodes
TRIQUAD_ARBT_BITS    number of arbitration bits
Number of nodes must be known at compile time. Number of arbitration bits
must satisfy: (2 ^ TRIQUAD_ARBT_BITS) >= TRIQUAD_NUM_NODES

DYNAMIC_RANK
Whether message arbitration is based on fixed node IDs or time-varying
priorities (least-recently-used algorithm)

HALT_ON_ERROR
Infinite nop loop if incorrect message (metadata and/or data) received

TRIQUAD_STATS
Controls whether protocol libary returns information about
arbitration, and this testbed collates same.

REPORT_AT = n
For running under GDB, etc. Breakpoint/dprintf to report progress and
stats every "n" messages. Must be (2^n)-1 value.

RESET_AT = n
Reset communication protocol hardware lines every "n" messages. Not
fully implemented in protocol libraries. Must be (2^n)-1 value.

RANDOM_DELAY_US = n
Microseconds to delay between sending messages when protocol
otherwise allows doing so. Set to zero for maximum stress testing of
protocol arbitration; to positive value to increase probability of
protocol library code coverage testing of non-contended node message
sending (one node sends when no others are).

BUSY_MASK = 0xNNN..
Mask LSBs of randomly generated number for length of busy loop to
simulate actual client being unresponsive to protocol participation.
See README.md for discussion re: POLLING/INTERRUPTS/PROGRESS

TRIQUAD_ALRT_ISR
#define'd string to match entry in ARM MCU vendor-specific SCB->VTOR
NVIC table

*/


#if !defined(TRIQUAD_TRI2B) && !defined(TRIQUAD_QUAD4ME)
#error Define one of TRIQUAD_TRI2B or TRIQUAD_QUAD4ME
#endif

#if defined(TRIQUAD_INTERRUPTS) && defined(TRIQUAD_BIT_BY_BIT)
#error Only define TRIQUAD_BIT_BY_BIT if TRIQUAD_POLLING
#endif


#include <stdint.h>

#include <bitops.hxx>
#include <xorshift_random.hxx>

#include <mcu.hxx>
#ifdef DYNAMIC_RANK
#include <rank_id.hxx>
#endif

#include <randomtest_port.inl>

#include "mcu_config.hxx"




#ifndef TRIQUAD_NUM_NODES
#error must define TRIQUAD_NUM_NODES
#endif

#ifndef TRIQUAD_ARBT_BITS
#error must define TRIQUAD_ARBT_BITS
#endif

#ifndef TRIQUAD_ID
#error must define TRIQUAD_ID
#endif


#if TRIQUAD_NUM_NODES > (1 << TRIQUAD_ARBT_BITS)
#error TRIQUAD_ARBT_BITS insufficient to encode TRIQUAD_NUM_NODES
#endif

#define HALT_ON_ERROR



namespace randomtest {

#ifdef TRIQUAD_INTERRUPTS
#ifdef TRIQUAD_TRI2B
#define TRIQUAD_ALRT_ISR    TRI2B_ALRT_ISR
const uint32_t              NVIC_PROTOCOL_START_BIT =   tri2b_config
                                                      ::ALRT_NVIC_BIT;
#endif
#ifdef TRIQUAD_QUAD4ME
#define TRIQUAD_ALRT_ISR    QUAD4ME_ALRT_ISR
const uint32_t              NVIC_PROTOCOL_START_BIT =   quad4me_config
                                                      ::ALRT_NVIC_BIT;
#endif
#endif  // #ifdef TRIQUAD_INTERRUPTS


// Promote #define'd strings for access in GDB
//
static const char* const    T2B_QQ4M_STRING __attribute__((used)) =
#ifdef TRIQUAD_TRI2B
                            "TRI2B";
#endif
#ifdef TRIQUAD_QUAD4ME
                            "QUAD4ME";
#endif

static const char* const    INTRPT_POLL_STRING  __attribute__((used)) =
#ifdef TRIQUAD_POLLING
                            "POLLING";
#endif
#ifdef TRIQUAD_INTERRUPTS
                            "INTERRUPTS";
#endif

static const char* const    PROTOCOL_STRING __attribute__((used)) =
#ifdef TRIQUAD_BIT_BY_BIT
                            "BIT BY BIT";
#endif
#ifdef TRIQUAD_WHOLE_MESSAGE
                            "WHOLE MESSAGE";
#endif

static const char* const    PROGRESS_STRING __attribute__((used)) =
#ifdef TRIQUAD_PROGRESS
#ifndef TRIQUAD_POLLING
#error Only define TRIQUAD_PROGRESS with TRIQUAD_POLLING
#endif
                            "PROGRESS TRACKED";
#else
                            "PROGRESS IGNORED";
#endif

static const char* const    STATS_STRING __attribute__((used)) =
#ifdef TRIQUAD_STATS
                            "STATS ON" ;
#else
                            "STATS OFF";
#endif

static const char* const    DYN_RNK_STRING __attribute__((used)) =
#ifdef DYNAMIC_RANK
                            "DYNAMIC RANK";
#else
                            "STATIC RANK" ;
#endif

static const char* const    NXP_GLITCH_FILTER_STRING __attribute__((used)) =
#ifdef TRIQUAD_NXP_GLITCH_FILTER
                            "ON";
#else
                            "OFF" ;
#endif



// constants from #define/Makefile (for runtime change in GDB)
// not const to allow changing in  GDB
uint32_t    RANDOM_DELAY_K = RANDOM_DELAY_US,    // delay_mask computed at runtime
            REPORT_AT_K    = REPORT_AT      ,
            RESET_AT_K     = RESET_AT       ,
            BUSY_MASK_K    = BUSY_MASK      ;

// message information history, most recent HISTORY_LEN messages sent
//   or received
// must match, and must be power of 2
const uint8_t   HISTORY_LEN = 32;
const uint32_t  HISTORY_MSK = 0x1f;

// message to be sent when protocol and/or random_delay ready
volatile uint32_t   pend_data,
                    pend_meta;

// flag not to re-check send_ready() if already done (TRIQUAD_INTERRUPTS)
volatile bool   send_pend = false;


// statistics variables
// depending on compile-time #defines some not used, but all (most)
//   always included for access in GDB
volatile uint32_t   num_sends    = 0,  // number of messages sent
                    num_recvs    = 0,  //   "    "     "     received
                    num_mesgs    = 0,  //   "    "     "     sent+received
                    num_ntrs     = 0,  // number of interrupts
                    num_prts     = 0,  // number of protocol() calls w/message
                    num_bits     = 0,  // total number of bits in all num_mesgs
                    num_redys    = 0,  // number of times random_delay ready
                    num_pends    = 0,  //   "    "    "   pending data ready
                    num_timrs    = 0,  //   " times random_delay timer running
                    pend_only    = 0,  // # times pending and timer not running
                    timr_only    = 0,  // # times timer running and not pending
                    num_wnnrs    = 0,  // number of times send won arbitration
                    num_nzwnr    = 0,  // number of times send won  " at _rank>0
                    num_losrs    = 0,  //   "    "    "    "   lost     "
                    num_rcvrs    = 0,  //   "    "    "   receive, no arbitration
                    num_loops    = 0,  // count of main loops
                    last_pend    = 0,  // mesg# at last set pending
                    last_send    = 0,  //  "    "   "   check_message("send")
                    last_recv    = 0,  //  "    "   "         "      ("recv")
#if REPORT_AT != 0
                    reported_at  = 0,  // last num_mesgs when reported
#endif
#if RESET_AT != 0
                    resetted_at  = 0,  // last num_mesgs resetted
#endif
                    num_errors   = 0;

int32_t             first_error  = -1;  // num_mesgs at first error


// history arrays -- information on last HISTORY_LEN messages
volatile uint16_t   msgnm[HISTORY_LEN] = {0};  // message number
volatile uint32_t   dirs               = 0x0;  // 1==send 0==receive
volatile uint32_t   datas[HISTORY_LEN] = {0};  // message data
volatile uint8_t    arbts[HISTORY_LEN] = {0},  // winning node arbitration rank
                    sndrs[HISTORY_LEN] = {0},  //    "     "        "      ID
                    metas[HISTORY_LEN] = {0};  // metadata




uint32_t            random_meta  = 0xa39eb80b,  // xor_shift PRNG seed
                    random_delay = 0xd021792a,  //     "      "    "
#if BUSY_MASK != 0
                    random_busy  = 0x9761ef66,  //     "      "    "
#endif
                    delay_mask   = 0         ;   // dynamically initted


// current PNRG sequence data, per node
// for self to send, and to check messages received from other nodes
uint32_t    random_datas[TRIQUAD_NUM_NODES];

// number of messages received from each other node, and sent by self
uint32_t    mesgs[TRIQUAD_NUM_NODES] = {0};

#ifdef DYNAMIC_RANK
rank_id::RankId<TRIQUAD_NUM_NODES, MIN_DYNAMIC_RANK, MAX_DYNAMIC_RANK>  rank_ids;
#endif

#ifdef TRIQUAD_STATS
void record_score()
{
    switch (triquad_random.scor()) {
        case TriquadBase::Scor::RCVR:
            ++num_rcvrs;
            break;

        case TriquadBase::Scor::NZWN:
            ++num_nzwnr;
            [[gnu::fallthrough]];  // silence compiler warning)

        case TriquadBase::Scor::WNNR:
            ++num_wnnrs;
            break;

        case TriquadBase::Scor::LOSR:
            ++num_losrs;
            break;

        case TriquadBase::Scor::PEND:
            break; // impossible
    }
}
#endif



void init_seeds()
{
    for (unsigned   sender = 0                         ;
                    sender <   sizeof(random_datas   )
                             / sizeof(random_datas[0]) ;
                  ++sender                              ) {

        // start each at different place in PRNG sequence
        random_datas[sender] = random_meta + random_delay;

        bitops::xor_shift(random_meta );
        bitops::xor_shift(random_delay);
    }

    // make each node's meta and delay seeds different
    random_meta  += random_datas[triquad_random.id()];
#if RANDOM_DELAY_US > 0
    random_delay += random_datas[triquad_random.id()];

    // no modulo (division) in low-end ARM chips (M0, M0+)
    // have to do by power-of-2 bitmask
    // calculate (delay_mask - 1) >= random_delay
    delay_mask = 0x1;
    while (delay_mask < RANDOM_DELAY_K)
        delay_mask <<= 1;
    delay_mask -= 1;
#endif
}



// called after each message, sent or received, from main loop
//   or interrupt handler
void check_message(
const char* const   send_or_recv)
{
    volatile char* volatile       s_or_r  // for GDB
                                = const_cast<volatile char*>(send_or_recv);
             uint8_t            arbt = triquad_random.arbt(    ),
                                meta = triquad_random.meta(    ),
#ifdef DYNAMIC_RANK
                                sndr = rank_ids.id        (arbt);
#else
                                sndr = arbt                     ;
#endif
    uint32_t    data = triquad_random.recvbuf();

    // circular buffers
    msgnm[num_mesgs & HISTORY_MSK] = num_mesgs;
    arbts[num_mesgs & HISTORY_MSK] = arbt;
    sndrs[num_mesgs & HISTORY_MSK] = sndr;
    metas[num_mesgs & HISTORY_MSK] = meta;
    datas[num_mesgs & HISTORY_MSK] = data;

    dirs = (dirs << 1);

    if (send_or_recv[0] == 's') {
        ++num_sends          ;
        dirs |= 0x1          ;
        last_send = num_mesgs;
    }
    else {
        ++num_recvs          ;
        last_recv = num_mesgs;
    }

    ++num_mesgs  ;
    ++mesgs[sndr];

    num_bits += meta;  // for this testbed -- actual client likely different

#ifdef TRIQUAD_STATS
    record_score();
#endif

    check:
    asm("nop");   // for GDB dprintf

    if (bitops::num_lsbs(data) != meta) {  // send_data() guarantees are ==
#ifdef HALT_ON_ERROR
        while (true)
            asm("nop");
#else
        if (first_error < 0)
            first_error = num_mesgs;
        ++num_errors;
#endif
    }

#ifndef DYNAMIC_RANK
    // can't do this with dynamic rank because post-arbitration rank may
    //   have changed and is now same as winner's previous rank
    if (arbt == triquad_random.rank()) {
#ifdef HALT_ON_ERROR
        while (true)
            asm("nop");
#else
        if (first_error < 0)
            first_error = num_mesgs;
        ++num_errors;
#endif
    }
#endif

    // known PNRG sequence
    if ((random_datas[sndr] & bitops::lsbs_mask(meta)) != data) {
#ifdef HALT_ON_ERROR
        while (true)
            asm("nop");
#else
        if (first_error < 0)
            first_error = num_mesgs;
        ++num_errors;
#endif
    }

    bitops::xor_shift(random_datas[sndr]);  // step PNRG

#ifdef DYNAMIC_RANK  // see rank_id.hxx for description
    rank_ids.re_rank(arbt);
#endif

#if REPORT_AT != 0
    if (   (num_mesgs & REPORT_AT_K) == 0
        && reported_at != num_mesgs      ) {  // don't do at num_mesgs==0
        reported_at = num_mesgs;
        results:
        asm("nop");   // GDB dprintf
    }
#endif

} // check_message()



bool send_ready() {
    bool    ready = false;

    if (triquad_random.pending()) {
        ++num_pends;
#if RANDOM_DELAY_US > 0
        if (!random_delay_timer_is_running())
            ++pend_only;
#endif
    }

#if RANDOM_DELAY_US > 0
    if (random_delay_timer_is_running()) {
        ++num_timrs;
        if (!triquad_random.pending())
            ++timr_only;
    }
#endif

    if (   !triquad_random.pending()
#if RANDOM_DELAY_US > 0
        && !random_delay_timer_is_running()
#endif
                                           ) {
        ++num_redys;
        ready     = true;
        send_pend = true;
#if RANDOM_DELAY_US > 0   // restart timer
        uint32_t    delay_us = bitops::xor_shift(random_delay) & delay_mask;

        if (delay_us == 0) delay_us = 1;   // arm::SysTick on STM can't do 0

        random_delay_timer_one_shot(microseconds_to_clocks(delay_us));
#endif
    }
    return ready;
}


void send_data() {
    uint8_t     width = (bitops::xor_shift(random_meta) & META_MSK) + 1;

    pend_data = random_datas[triquad_random.id()] & bitops::lsbs_mask(width);
    pend_meta = bitops::num_lsbs(pend_data);

    triquad_random.pending(pend_data, pend_meta);

    send_pend = false;
    last_pend = num_mesgs;
}

} // namespace randomtest



// simple implementation
// actual client likely non-linear mapping, e.g. small number of
//    message types each of known, fixed size
uint8_t TriquadBase ::meta2bits(
uint8_t meta)
{
    return meta;
}



using namespace randomtest;

#ifdef TRIQUAD_INTERRUPTS
// "TRIQUAD_ALRT_ISR" is #define'd string to match entry in ARM MCU
//   vendor-specific SCB->VTOR NVIC table
// See README.md for rationale for time-consuming interrupt handler
extern "C" void __attribute__ ((isr)) TRIQUAD_ALRT_ISR()
{
    ++num_ntrs;

    // must be done here, otherwise one or more other nodes can
    // interrupt-storm and this node will infrequently or never return
    // to main loop to check send_ready() and call send_data()
    if (send_pend || send_ready())
        send_data();

    // only count time in actual protocol, unfortunately not including ISR latency
    statistics_timer.resume();

    if (triquad_random.protocol()) {
        statistics_timer.pause();  // only time protocol
        check_message(  triquad_random.role() == TriquadBase::Role::SENDER
                      ? "send"
                      : "recv");
        statistics_timer.resume();  // only time protocol
        ++num_prts;
    }

    statistics_timer.pause();  // if !protocol(), otherwise redundant+effectless
}
#endif   // ifdef TRIQUAD_INTERRUPTS



// program
//
int main()
{
    init_seeds();

    randomtest_init();

    triquad_random.init();

#ifdef DYNAMIC_RANK
    rank_ids.init();
#endif

#ifdef TRIQUAD_PROGRESS
    TriquadBase::State  prev_state = TriquadBase::State::WRIT;
#endif

    // paused and resumed to measure only actual protocol execution time
    // will never be reset
    // milliseconds, 32 bits, will wrap in 49.71... days
    statistics_timer.one_shot(TIMER_MAX);
    statistics_timer.pause   (         );


    triquad_random.reset();  // init

    // hack to reset timebase of random_delay_timer if reused/same
    // hardware timer as reset_timer
    triquad_random.post_reset();


    // run
    while (true) {
#if BUSY_MASK != 0  // busy any time
        bitops::xor_shift(random_busy);
        for (uint32_t count = 0 ; count < (random_busy & BUSY_MASK_K) ; ++count)
            asm("nop");
#endif

#if RESET_AT != 0  // don't use -- not fully implemented in protocol libraries
        if (      (num_mesgs & RESET_AT_K) == 0
            &&     num_mesgs               != resetted_at
            && (   triquad_random.state()  == TriquadBase::State::IDLE
                || triquad_random.state()  == TriquadBase::State::FINISH)) {
            resetted_at = num_mesgs;
            triquad_random.reset() ;
        }
#endif

#ifdef TRIQUAD_POLLING
        statistics_timer.resume(); // only measure actual protocol execution time
        bool    message_finished = triquad_random.protocol();
        statistics_timer.pause();

        if (message_finished)
            check_message(  triquad_random.role() == TriquadBase::Role::SENDER
                          ? "send"
                          : "recv");

#if defined(TRIQUAD_BIT_BY_BIT) && defined(TRIQUAD_PROGRESS)
        // simulate actual client skipping other tasks to give priority
        // to protocol
        if (triquad_random.state() == prev_state)
            continue;  // priority to triquad
        else
            prev_state = triquad_random.state();
#endif
#endif

        ++num_loops;

#ifdef TRIQUAD_POLLING
        if (send_ready())
            send_data();
#endif

#ifdef TRIQUAD_INTERRUPTS
        // trigger interrupt -- do not call ISR directly
        if (send_ready())
            NVIC->ISPR[0] = NVIC_PROTOCOL_START_BIT;
#endif
    }
}  // main()
