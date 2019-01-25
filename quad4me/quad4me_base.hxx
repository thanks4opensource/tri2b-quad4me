#ifndef QUAD4ME_BASE_HXX
#define QUAD4ME_BASE_HXX

#include <tri_quad.hxx>

// see README.md for descriptions and discussion
//

namespace triquad {

class Quad4meBase : public TriQuad {
  public:
    constexpr
    Quad4meBase(
    const uint8_t     num_nodes,
    const uint8_t     arbt_bits,
    const uint8_t     meta_bits,
    const uint8_t     node_id  )
    :   TriQuad(num_nodes, arbt_bits, meta_bits, node_id)
    {}

    void    reset();


  protected:
    // implemented in and lnked from hardware-specific Tri2b derived class
    friend class TriQuad;

    inline void     set_cycl(),
                    clr_cycl();
    inline bool         cycl() volatile const;
    // end of implemented in and lnked from hardware-specific Tri2b derived class

};
} // namespace triquad

#endif   // ifndef QUAD4ME_BASE_HXX
