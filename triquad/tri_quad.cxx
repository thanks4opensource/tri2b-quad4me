#include <tri_quad.hxx>



namespace triquad {

void TriQuad::reset()
{
    // see README.md for description/discussion

    // wait again
    reset_delay_start();
    while (reset_delay_wait())
        asm("nop");

    _phase = Phase::IDLE;

}  // void TriQuad::reset()



#ifdef DYNAMIC_RANK
void TriQuad::dynamic_rank()
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

}  // void TriQuad::dynamic_rank()
#endif  // ifdef DYNAMIC_RANK



#ifdef TRIQUAD_STATE_STRINGS
const char  *TriQuad::_PHASE_STRINGS[] = {
        "ID",
        "AR",
        "ME",
        "DA",
};
const char  *TriQuad::_ROLE_STRINGS[] = {
        "S",
        "R",
};
#endif   // ifdef TRIQUAD_STATE_STRINGS
#ifdef TRIQUAD_STATS
const char  *TriQuad::_SCOR_STRINGS[] = {
        "PND",
        "RCV",
        "WIN",
        "NZW",
        "LOS",
};
#endif   // ifdef TRIQUAD_STATS

} // namespace tri2b
