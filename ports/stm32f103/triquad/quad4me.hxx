#ifndef QUAD4ME_HXX
#define QUAD4ME_HXX

#include <quad4me_base.hxx>



namespace triquad {

class Quad4me : public Quad4meBase {
  public:
    constexpr
    Quad4me(
    // base
    const uint8_t   num_nodes     ,
    const uint8_t   num_arbt_bits ,
    const uint8_t   num_meta_bits ,
    const uint8_t   node_id)
    :   Quad4meBase(num_nodes     ,
                    num_arbt_bits ,
                    num_meta_bits ,
                    node_id       )
    {}

    void    init (),
            reset();

}; // class Quad4me

} // namespace triquad

#endif   // ifndef quad4me_hxx
