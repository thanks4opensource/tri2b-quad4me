#ifndef XORSHIFT_RANDOM_HXX
#define XORSHIFT_RANDOM_HXX

namespace bitops {

// client holds state
inline uint32_t xor_shift(
uint32_t &state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state <<  5;
    return  state;
}



class XorShift {
  public:
    constexpr
    XorShift(
    uint32_t    seed = 1)   // must be non-zero
    :   _state(seed)
    {}


    uint32_t operator()() {
        _state ^= _state << 13;
        _state ^= _state >> 17;
        _state ^= _state <<  5;
        return _state;
    }


  protected:
    uint32_t    _state;

};

} // namespace bitops

#endif   // ifndef XORSHIFT_RANDOM_HXX
