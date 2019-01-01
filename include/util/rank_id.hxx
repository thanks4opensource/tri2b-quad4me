#ifndef RANK_ID_HXX
#define RANK_ID_HXX

/*
Maintain mapping of ranks to IDs

Simple least-recently-used (LRU) algorithm.

Bounds for range of IDs participating in dynamic ranking.

Arbitration winner rank becomes MAX_DYNAMIC_RANK,
all others with rank > winner and rank <= MAX_DYNAMIC_RANK decrement rank by 1

Only used if need to access mapping. Individual IDs simply set own rank to
MAX_DYNAMIC_RANK if win arbitration and rank >= MIN_DYNAMIC_RANK,
decrement rank by 1 otherwise

Initial state: rank == ID

Example:
MIN_DYNAMIC_RANK = 2
MAX_DYNAMIC_RANK = 8

arbt #      1       2       3       4       5       6       7       8       9
rank                                ID
 0          0       0       0       0       0       0       0       0       0
 1          1       1       1       1win    1       1       1       1       1
 2          2       2       2       2       2       2win    5       5       5
 3          3       3       3win    5       5       5       6       6       6
 4          4win    5       5       6       6       6       8       8       8
 5          5       6       6       8       8       8       4       4       4
 6          6       7win    8       4       4       4       7       7       7
 7          7       8       4       7       7       7       3       3win    2
 8          8       4       7       3       3       3       2win    2       3
 9          9       9       9       9       9win    9       9       9       9

*/



#include <stdint.h>

namespace rank_id {

template <uint8_t NUM_NODES, uint8_t MIN_DYN_RANK, uint8_t MAX_DYN_RANK>
class RankId {
  public:
    void init()
    {
        for (uint8_t ndx = 0 ; ndx < NUM_NODES ; ++ndx)
            _rank_to_id[ndx] = ndx;
    }

#ifdef DYNAMIC_RANK
    uint8_t id(const uint8_t rank) const { return _rank_to_id[rank]; }

    void re_rank(
    const uint8_t   rank)
    {
        if (rank < MIN_DYN_RANK || rank > MAX_DYN_RANK)
            return;

        const uint8_t   id = _rank_to_id[rank];

        for (uint8_t ndx = rank ; ndx < MAX_DYN_RANK ; ++ndx)
            _rank_to_id[ndx] = _rank_to_id[ndx + 1];

        _rank_to_id[MAX_DYN_RANK] = id;
    }

#else
    uint8_t id(const uint8_t rank) const { return rank; }

    void re_rank(const  uint8_t rank) {}
#endif


  protected:
    uint8_t     _rank_to_id[NUM_NODES];

};

} // namespace rank_id

#endif // ifndef RANK_ID_HXX
