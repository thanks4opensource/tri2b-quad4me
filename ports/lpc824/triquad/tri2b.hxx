#ifndef TRI2B_HXX
#define TRI2B_HXX

#include <tri2b_base.hxx>



namespace triquad {

class Tri2b : public Tri2bBase {
  public:
    constexpr
    Tri2b(
    // base
    const uint8_t   num_nodes     ,
    const uint8_t   num_arbt_bits ,
    const uint8_t   num_meta_bits ,
    const uint8_t   node_id)
    :   Tri2bBase(num_nodes   ,
                    num_arbt_bits ,
                    num_meta_bits ,
                    node_id       )
    {}

    void    init (),
            reset();

}; // class Tri2b

} // namespace triquad

#endif   // ifndef tri2b_hxx
