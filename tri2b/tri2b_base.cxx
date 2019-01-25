/*

    Protocol:

    DATA  :X::?::X::?::X::?::X::?::X::?::X::?::X::?::X::?:::::
          W      W     W     W     W     W     W     W     W
    ALRT  ~~\___/~\___/~\___/~\___/~\___/~\___/~\___/~\___/~~~
              R     R     R     R     R     R     R     R
    LTCH  ___/~\___/~\___/~\___/~\___/~\___/~\___/~\___/~\____

    Legend:
    :    data line, either high or low
    X    data line, data bit written: high->high, high->low, low->high, low->low
    ?    data line, data bit read
    ~    line high
    _    line low
    /    line, low->high
    \    line, high->low
    R    READ State
    W    WRIT State

    See README.md for more detail.

*/


#include <tri2b_base.hxx>

#include <tri2b.inl>

#include <tri2b_config.hxx>




#ifdef TRIQUAD_BIT_BY_BIT
#define RETURN_OR_CONTINUE  return false
#endif
#ifdef TRIQUAD_WHOLE_MESSAGE
#define RETURN_OR_CONTINUE  continue
#endif



namespace triquad {

void Tri2bBase::reset()
{
    // see README.md for description/discussion

    // wait for all nodes to do same (all lines high)
    while (!data() || !alrt() || !ltch())
        asm("nop");

    // wait for sync delay time or until another node goes first
    reset_delay_start();
    while (ltch() && reset_delay_wait())
        asm("nop");

    // if first node to get here will break others out of sync wait loop above
    clr_ltch();

    TriQuad::reset();  // finish reset protocol

}  // void Tri2bBase::reset()



bool TriQuad::protocol()
{
    // returns true when message complete (send or receive)
    // ifdef TRIQUAD_BIT_BY_BIT    returns false when message in progress
    // ifdef TRIQUAD_WHOLE_MESSAGE does not return until message complete
    //
    // see README.md for details


    if (_phase == Phase::IDLE) {
        // preliminary Phase check
        // only for IDLE
        // check to see if start of new message, either self pending or
        //    or other nodes sending
        // not done in no-return-until-message-complete main loop, WHOLE_MESSAGE mode
        // cannot be in switch(_phase) after switch(_state) because READ/WRITE
        //   _state checks are different for IDLE vs ARBT/META/DATA

#ifdef TRIQUAD_INTERRUPTS
        clr_alrt_fall();
#endif

        // check alrt low (other node(s) sending) or message ready to send
        //
        // do even in TRIQUAD_INTERRUPTS mode because can have interrupt
        // between pending() check and explicit invocation of protocol()
        // and then back here after interrupt return
        if (alrt() && !_pending)
            return false;

#ifdef TRIQUAD_INTERRUPTS
        disble_alrt_fall();
#endif

        // clear any pending rising edges
        clr_alrt_rise();
        clr_ltch_rise();

        // signal message start if first/only node with _pending==true
        // redundant if not
        // but still required for message begin protocol _state
        clr_alrt();

        // init message
        _arbt    = 0x0           ;
        _arbtbit = _ARBT_BITS - 1;  // MSB
        _bit     = 1 << _arbtbit ;  //  "
        _recvbuf = 0             ;

        if (_pending) {
            _sendbuf = _rank;  // to compete in arbitration

            if (_sendbuf & _bit) set_data();
            else                 clr_data();

            _role = Role::SENDER;
            TQ_SCOR(Scor::PEND) ;
        }
        else {
            _sendbuf = ALL_HIGH;  // not competing in arbitration

            set_data();

            _role = Role::RECVER;
            TQ_SCOR(Scor::RCVR) ;
        }

        set_ltch();

        _phase = Phase::ARBT;
        _state = State::READ;
    }

#ifdef TRIQUAD_STATS
    _systick_start = arm::SysTick::count();
#endif

    while (true) {
    // will return if BIT_BY_BIT and no protocol progress,
    // remain until complete if WHOLE_MESSAGE

        loop: asm("nop");  // for GDB dprintf -- GDB bug prevents from working


        if (_state == State::READ) {
            if (!ltch_rise()) RETURN_OR_CONTINUE;

            TQ_WAITS;

            clr_ltch_rise();

            _recvbuf   = (_recvbuf << 1) | data();  // receive bit
            _bit     >>= 1;                         // prepare for next bit

            // step protocol
            clr_ltch();

            _state = State::WRIT;
            // fall through to switch (_phase)
        }

        else {  // _state == State::WRIT
            if (!alrt_rise()) RETURN_OR_CONTINUE;

            TQ_WAITS;

            clr_alrt_rise();

            if (_sendbuf & _bit) set_data();
            else                 clr_data();

            // step protocol
            clr_alrt();
            set_ltch();

            _state = State::READ;

            RETURN_OR_CONTINUE;
        }


        switch (_phase) {
            case Phase::IDLE:
                // impossible, handled above.
                // keep compiler happy
                break;

            case Phase::ARBT:
                set_alrt();  // can't be done (always) in State::READ

                if (_recvbuf != static_cast<uint32_t>(_rank >> _arbtbit)) {
                    // have lost arbitration: own bit was 1, one or more
                    // other nodes' bit(s) are zero
                    //
                    // if receiver (not competing in arbitration) will
                    // eventually lose unless winning node rank is
                    // all ones, but effect-less because already Role::RECVER
                    //
                    _role = Role::RECVER;
                    _sendbuf = ALL_HIGH;
#ifdef TRIQUAD_STATS
                    if (_scor == Scor::PEND) _scor = Scor::LOSR;
#endif
                }

                _arbtbit -= 1;  // decrements one too many, but unused afterward

                if (!_bit) {  // arbitration finished
                    if (_role == Role::SENDER) {
                        _sendbuf = _pendmet  ;
#ifdef TRIQUAD_STATS
                        // must be before call to dynamic_rank()
                        if (_rank) _scor = Scor::NZWN;
                        else       _scor = Scor::WNNR;
#endif
                    }
                    else  // _role == Role::RECVR)
                        _sendbuf = ALL_HIGH;

                    _arbt = _recvbuf;  // must be before dynamic_rank()
#ifdef DYNAMIC_RANK
                    static_cast<TriQuad*>(this)->dynamic_rank();  // adjust rank
#endif
                    _recvbuf = 0                ;  // reset for META phase
                    _bit = 1 << (_META_BITS - 1);  //   set for META phase
                    _phase = Phase::META        ;
                }
                break;


            case Phase::META:
                set_alrt();  // can't be done (always) in State::READ

                if (!_bit) {  // meta finished
                    if (_role == Role::SENDER)
                        _sendbuf = _pendbuf;
                    // else _role == Role::RECVR, _sendbuf already ALL_HIGH
                    _meta = _recvbuf;
                    _recvbuf = 0                      ;  // reset for DATA phase
                    _bit = 1 << (meta2bits(_meta) - 1);  //   set for DATA phase
                    _phase = Phase::DATA              ;
                }
                break;


            case Phase::DATA:
                if (!_bit) {  // data, and whole message, complete
                    if (_role == Role::SENDER)
                        _pending = false;

#ifdef TRIQUAD_INTERRUPTS
                    // before final set_alrt() to be ready for other
                    // nodes starting next message
                    enable_alrt_fall();
#endif


                    // reset to known condition for wait-for-finish loop below
                    // must be before set_alrt(), otherwise race condition
                    // if TRIQUAD_DATA_WAIT_US > 0 and set_data() is waiting
                    // for actual high on line but another node gets ahead
                    // into setting arbitration bit low at IDLE/ARBT
                    set_alrt();

                    clr_alrt_rise();  // in case all others high, don't want

                    // wait for all nodes to finish message
                    while (!(alrt() && !ltch()))
                        asm("nop");
                    set_data();
                    _phase = Phase::IDLE;

                    MESGS;
                    return true;
                }

                set_alrt();  // can't be done (always) in State::READ

                break;
        }  // switch (_phase)

#ifdef TRIQUAD_BIT_BY_BIT  // continue until message complete if WHOLE_MESSAGE
        return false;
#endif
    }  // while (true)

}  // bool TriQuad::protocol()



#ifdef TRIQUAD_STATE_STRINGS
const char  *TriQuad::_STATE_STRINGS[] = {
        "RD",
        "WR",
};
#endif   // ifdef TRIQUAD_STATE_STRINGS

} // namespace triquad
