#ifndef TRI2B_BASE_HXX
#define TRI2B_BASE_HXX

#include <tri_quad.hxx>

// see README.md for descriptions and discussion
//

namespace triquad {

class Tri2bBase : public TriQuad {
  public:
    constexpr
    Tri2bBase(
    const uint8_t     num_nodes,
    const uint8_t     arbt_bits,
    const uint8_t     meta_bits,
    const uint8_t     node_id   )
    :   TriQuad(num_nodes, arbt_bits, meta_bits, node_id)
#if TRIQUAD_MIN_HIGH_US > 0
                            ,
        _min_high_start  (0)
#endif
    {}

    void    reset();


  protected:
    // implemented in and lnked from hardware-specific Tri2b derived class
    friend class TriQuad;

#if TRIQUAD_MIN_HIGH_US > 0
    inline void     start_min_high(),
                     wait_min_high();
#endif
    // end of implemented in and lnked from hardware-specific Tri2b derived class


#if TRIQUAD_MIN_HIGH_US > 0
    uint32_t    _min_high_start;
#endif
};
} // namespace triquad

#endif   // ifndef TRI2B_BASE_HXX
