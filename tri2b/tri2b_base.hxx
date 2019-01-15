#ifndef TRI2B_BASE_HXX
#define TRI2B_BASE_HXX

#include <stdint.h>

#include <bitops.hxx>



// see README.md for descriptions and discussion
//

// see protocol(), below
#if !defined(TRIQUAD_BIT_BY_BIT) && !defined(TRIQUAD_WHOLE_MESSAGE)
#error Define one of TRIQUAD_BIT_BY_BIT or TRIQUAD_WHOLE_MESSAGE
#endif
#if  defined(TRIQUAD_BIT_BY_BIT) &&  defined(TRIQUAD_WHOLE_MESSAGE)
#error Define only one of TRIQUAD_BIT_BY_BIT or TRIQUAD_WHOLE_MESSAGE
#endif

#if !defined(TRIQUAD_INTERRUPTS) && !defined(TRIQUAD_POLLING)
#error Define one of TRIQUAD_INTERRUPTS or TRIQUAD_POLLING
#endif
#if  defined(TRIQUAD_INTERRUPTS) &&  defined( TRIQUAD_POLLING)
#error Define only one of TRIQUAD_INTERRUPTS or TRIQUAD_POLLING
#endif


#ifdef DYNAMIC_RANK
#if !defined(MIN_DYNAMIC_RANK) || !defined(MAX_DYNAMIC_RANK)
#error Define both MIN_DYNAMIC_RANK and MAX_DYNAMIC_RANK
#endif
#endif



namespace tri2b {

class Tri2bBase {
  public:

    // see README.md for details of protocol States, Phases, Roles
    //
    enum class State {
        READ = 0,   // for efficient "if" test
        WRIT = 1,   // "else"
    };

    enum class Phase {
        IDLE = 0,   // for efficient "if" test
        ARBT,
        META,
        DATA,
    };

    enum class Role {
        SENDER = 0, // for efficient "if" test
        RECVER = 1  // "else"
    };

#if defined(TRIQUAD_STATS) || defined(TRIQUAD_EXTRA_MEMBERS)
    enum class Scor {
        PEND = 0,
        RCVR,
        WNNR,
        NZWN,  // non-zero winner (win with _rank != 0)
        LOSR,
    };
#endif

    constexpr
    Tri2bBase(
    const uint8_t     num_nodes,
    const uint8_t     arbt_bits,
    const uint8_t     meta_bits,
    const uint8_t     node_id   )
    :   _sendbuf         (0xffffffff ),   // default receiver arbitration
        _recvbuf         (0          ),
        _bit             (0x00000001 ),   // init, prelude all 1, _sendbuf&0x1
        _pendbuf         (0          ),
        _meta            (0          ),
        _phase           (Phase::IDLE),
        _state           (State::WRIT),
        _role            (Role::RECVER),
#if TRIQUAD_MIN_HIGH_US > 0
        _min_high_start  (0           ),
#endif
#if defined(TRIQUAD_STATS) || defined(TRIQUAD_EXTRA_MEMBERS)
        _scor            (Scor::RCVR ),
        _systick_start   (0          ),
        _waits           (0          ),
        _data_waits      (0          ),
        _data_timeouts   (0          ),
#endif
        _NUM_NODES       (num_nodes  ),
        _ARBT_BITS       (arbt_bits  ),
        _META_BITS       (meta_bits  ),
        _NODE_ID         (node_id    ),
#ifdef DYNAMIC_RANK                      // else use _NODE_ID for arbitration
        // numerically lower == higher priority
        _rank            (node_id    ),  // see rank(), below
#endif
#if TRIQUAD_DATA_WAIT_US > 0
        _prev_data       (0          ),  // only if data 0->1 change
#endif
        _pendmet         (0          ),
        _pending         (false      ),
        _arbt            (0          ),
        _arbtbit         (0          )   // single bit mask, MSB->LSB
    {}

    void    init() {}

    void    reset     (),
            post_reset();

    // returns true when message complete (send or receive)
    // ifdef TRIQUAD_BIT_BY_BIT    returns false when message in progress
    // ifdef TRIQUAD_WHOLE_MESSAGE does not return until message complete
    bool    protocol();

    uint8_t id  () const { return _NODE_ID; }
    uint8_t rank() const {
#ifdef DYNAMIC_RANK
        return _rank;
#else
        return _NODE_ID;
#endif
    }
    Phase   phase() const { return _phase; }
    Role    role () const { return _role ; }
    State   state() const { return _state; }

    uint8_t  arbt() const { return _arbt; }
    uint32_t meta() const { return _meta; }
#ifdef TRIQUAD_STATS
    Scor    scor() const { return _scor; }
#endif

    uint8_t     pendmet() const { return _pendmet; }
    uint32_t    sendbuf() const { return _sendbuf; }
    uint32_t    recvbuf() const { return _recvbuf; }
    bool        pending() const { return _pending; }

    void pending(
    const uint32_t  data,
    const uint8_t   meta)
    {
        _pendbuf = data;
        _pendmet = meta;
        _pending = true;  // must be last in case interrupt
    }

    // client needs to implement and link
    static uint8_t      meta2bits (uint8_t meta);

#ifdef TRIQUAD_INTERRUPTS
    // implemented in hardware-specific Tri2b derived class
           void     enable_interrupt();
    inline void     disble_alrt_fall(),
                    enable_alrt_fall();
#endif


  protected:
    static const uint32_t   ALL_HIGH = 0xffffffff;   // for receiver _sendbuf

#ifdef DYNAMIC_RANK
    void    dynamic_rank();
#endif

#ifdef TRIQUAD_STATE_STRINGS
    static const char   *_STATE_STRINGS[],
                        *_PHASE_STRINGS[],
                        * _ROLE_STRINGS[];
#endif
#ifdef TRIQUAD_STATS
    static const char   *_SCOR_STRINGS[];
#endif

#ifdef TRIQUAD_STATS
#define T2B_SCOR(SCOR)  _scor = SCOR
#define T2B_WAITS       waits_incr();
    inline void         waits_begn(),
                        waits_incr();
#else
#define T2B_SCOR(SCOR)
#define T2B_WAITS
#endif


    // implemented in and lnked from hardware-specific Tri2b derived class
    //

    void    reset_delay_start();
    bool    reset_delay_wait ();

#define MESGS

    inline void     set_alrt(),
                    clr_alrt(),
                    set_ltch(),
                    clr_ltch(),
#if TRIQUAD_DATA_WAIT_US == 0
                    set_data(),
#endif
                    clr_data();

#if TRIQUAD_DATA_WAIT_US > 0
           void     set_data();
#endif

    inline bool     alrt_rise(),
                    ltch_rise();

    inline void     clr_alrt_rise(),
                    clr_ltch_rise();

    inline bool     alrt() volatile const,
                    ltch() volatile const,
                    data() volatile const;

    // end of implemented in and lnked from hardware-specific Tri2b derived class


    // data members, used by both Tri2bBase and Tri2b derived class
          uint32_t  _sendbuf         ,
                    _recvbuf         ,
                    _bit             ,
                    _pendbuf         ;
          uint32_t  _meta            ;
          State     _state           ;
          Phase     _phase           ;
          Role      _role            ;
#if TRIQUAD_MIN_HIGH_US > 0
          uint32_t  _min_high_start  ;
#endif
#if defined(TRIQUAD_STATS) || defined(TRIQUAD_EXTRA_MEMBERS)
          Scor      _scor            ;
          uint32_t  _systick_start   ,
                    _waits           ,
                    _data_waits      ,
                    _data_timeouts   ;
#endif
    const uint8_t   _NUM_NODES       ,
                    _ARBT_BITS       ,
                    _META_BITS       ;
    const uint8_t   _NODE_ID         ;
#ifdef DYNAMIC_RANK
          uint8_t   _rank            ;
#endif
#if TRIQUAD_DATA_WAIT_US > 0
          uint8_t   _prev_data       ;
#endif
          uint8_t   _pendmet         ;
          bool      _pending         ;
          uint8_t   _arbt            ,
                    _arbtbit         ;
};
} // namespace tri2b

#endif   // ifndef TRIQUAD_BASE_HXX
