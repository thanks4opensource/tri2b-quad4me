/*

    Protocol:

        DATA  :X::?:::X:?:::X:?:::X:?:::X:?:::X:?:::X:?:::X:?:::::
                    N     N     N     N     N     N     N     N
        ALRT  ~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~~~
                  R     R     R     R     R     R     R     R
        LTCH  ___/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\___
               W      W     W     W     W     W     W     W     W
        CYCL  ~~~~\__/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~

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
        N    NEXT State

    See README.md for more detail.

*/



#include <quad4me_base.hxx>

#include <quad4me.inl>

#include <quad4me_config.hxx>




#ifdef TRIQUAD_BIT_BY_BIT
#define RETURN_OR_CONTINUE  return false
#endif
#ifdef TRIQUAD_WHOLE_MESSAGE
#define RETURN_OR_CONTINUE  continue
#endif



namespace quad4me {

void Quad4meBase::reset()
{
    // see README.md for description/discussion

    // wait for all nodes to do same (all lines high)
    while (!data() || !alrt() || !ltch() || !cycl())
        asm("nop");


    // wait for sync delay time or until another node goes first
    reset_delay_start();
    while (ltch() && reset_delay_wait())
        asm("nop");

    // if first node to get here will break others out of sync wait loop above
    clr_ltch();

    // wait again
    reset_delay_start();
    while (reset_delay_wait())
        asm("nop");

    _phase = Phase::IDLE;

#ifdef TRIQUAD_INTERRUPTS
    enable_interrupt();
    enable_alrt_fall();
#endif
}  // void Quad4meBase::reset()



bool Quad4meBase::protocol()
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

        // check alrt low (other node(s) sending) or message ready to send
        //
        // do even in TRIQUAD_INTERRUPTS mode because can have interrupt
        // between pending() check and explicit invocation of protocol()
        // and then back here after interrupt return
        if (alrt() && !_pending)
            return false;

        // signal message start if first/only node _pending==true
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
            Q4M_SCOR(Scor::PEND);
        }
        else {
            _sendbuf = ALL_HIGH;  // not competing in arbitration

            set_data();

            _role = Role::RECVER;
            Q4M_SCOR(Scor::RCVR);
        }

        set_ltch();

        _phase = Phase::ARBT;
        _state = State::READ;
    }

    while (true) {
    // will return if BIT_BY_BIT and no protocol progress,
    // remain until complete if WHOLE_MESSAGE

        loop: asm("nop");  // for GDB dprintf -- GDB bug prevents from working


        switch (_state) {
            case State::WRIT:
                if (!cycl()) {
                    Q4M_WAITS;
                    RETURN_OR_CONTINUE;
                }
                if (_sendbuf & _bit) set_data();
                else                 clr_data();

                // step protocol
                clr_alrt();
                set_ltch();

                _state = State::READ;

                RETURN_OR_CONTINUE;


            case State::READ:
                if (!ltch()) {
                    Q4M_WAITS;
                    RETURN_OR_CONTINUE;
                }

                _recvbuf   = (_recvbuf << 1) | data();
                _bit     >>= 1                       ;

                // step protocol
                clr_cycl();
                set_alrt();

                _state = State::NEXT;

                RETURN_OR_CONTINUE;


            case State::NEXT:
            if (!alrt()) {
                Q4M_WAITS;
                RETURN_OR_CONTINUE;
            }

            // step protocol
            clr_ltch();

            _state = State::WRIT;

            // fall through to switch (_phase)
            break;
        }


        switch (_phase) {
            case Phase::IDLE:
                // impossible, handled above.
                // keep compiler happy
                break;

            case Phase::ARBT:
                set_cycl();  // can't be done (always) in State::NEXT

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

                    _arbt = _recvbuf;  // must be before call to dynamic_rank()
#ifdef DYNAMIC_RANK
                    dynamic_rank();    // adjust _rank
#endif
                    _recvbuf = 0                ;  // reset for META phase
                    _bit = 1 << (_META_BITS - 1);  //   set for META phase
                    _phase = Phase::META        ;
                }
                break;


            case Phase::META:
                set_cycl();  // can't be done (always) in State::NEXT

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
                    // before final set_cycl() to be ready for other
                    // nodes starting next message
                    enable_alrt_fall();
#endif

                    // reset to known condition for wait-for-finish loop below
                    // must be before set_cycl(), otherwise race condition
                    // if TRIQUAD_DATA_WAIT_US != 0 and set_data() is waiting
                    // for actual high on line but another node goes ahead
                    // into setting arbitration bit low IDLE/ARBT
                    set_data();
                    set_cycl();

                    // wait for all nodes to finish message
                    while (!(cycl() && data() && alrt() && !ltch()))
                        asm("nop");

                    _phase = Phase::IDLE;

                    MESGS;
                    return true;
                }

                set_cycl();  // can't be done (always) in State::NEXT

                break;
        }  // switch (_phase)

#ifdef TRIQUAD_BIT_BY_BIT  // continue until message complete if WHOLE_MESSAGE
        return false;
#endif
    }  // while (true)

}  // bool Quad4meBase::protocol()



#ifdef DYNAMIC_RANK
void Quad4meBase::dynamic_rank()
{
    // least-recently used priority algorithm
    // numerically lower _rank == higher priority
    //
    // nodes with ranks in range [MIN_DYNAMIC_RANK, MAX_DYNAMIC_RANK]
    // change priority after arbitration finished:
    // if sender and won arbitration, move to MAX_DYNAMIC_RANK
    // else move numerically down one (up one in priority)
    //
    // client needs to use RankId class to track rank/arbt() to ID mapping
    //
    // #ifndef DYNAMIC_RANK, using unchanging _NODE_ID as _rank/priority

    if (
#if MIN_DYNAMIC_RANK > 0     // avoid compiler warning, uint>=0, always true
            _rank >= MIN_DYNAMIC_RANK
        &&
#endif
           _rank <= MAX_DYNAMIC_RANK)
        if (_rank == _arbt)     // winner: move to highest numeric
                                // value (lowest priority)
            _rank = MAX_DYNAMIC_RANK;
        else if (_rank > _arbt) // loser: lower numeric value
            _rank -= 1;         // (higher priority)
}  // void Quad4meBase::dynamic_rank()
#endif



#ifdef TRIQUAD_STATE_STRINGS
const char  *Quad4meBase::_STATE_STRINGS[] = {
        "RD",
        "WR",
        "NX",
};
const char  *Quad4meBase::_PHASE_STRINGS[] = {
        "ID",
        "AR",
        "ME",
        "DA",
};
const char  *Quad4meBase::_ROLE_STRINGS[] = {
        "S",
        "R",
};
#endif   // ifdef TRIQUAD_STATE_STRINGS
#ifdef TRIQUAD_STATS
const char  *Quad4meBase::_SCOR_STRINGS[] = {
        "PND",
        "RCV",
        "WIN",
        "NZW",
        "LOS",
};
#endif   // ifdef TRIQUAD_STATS
} // namespace quad4me
