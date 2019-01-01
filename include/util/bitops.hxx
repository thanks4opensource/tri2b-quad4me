#ifndef BITOPS_HXX
#define BITOPS_HXX

#include <stdint.h>

namespace bitops {

template <typename VOLATILITY>  static inline uint32_t GET_BITS(
VOLATILITY      &register_or_memory,
const uint32_t   bits)
{
    return register_or_memory & bits;
};


template <typename VOLATILITY> static inline void CHG_BITS(
VOLATILITY      &register_or_memory,
const uint32_t   mask,
const uint32_t   bits)
{
    register_or_memory = (register_or_memory & ~mask) | bits;
};


template <typename VOLATILITY> static inline void SET_BITS(
VOLATILITY      &register_or_memory,
const uint32_t   bits)
{
    register_or_memory |= bits;
};


template <typename VOLATILITY> static inline void CLR_BITS(
VOLATILITY      &register_or_memory,
const uint32_t   bits)
{
    register_or_memory &= ~bits;
};




#if !defined(BUILTIN_CLZ) && !defined(BINARY_SEARCH_CLZ)
// #error #define one of BUILTIN_CLZ or BINARY_SEARCH_CLZ
#define BINARY_SEARCH_CLZ
#endif

#if defined(BUILTIN_CLZ) && defined(BINARY_SEARCH_CLZ)
#error #define only one of BUILTIN_CLZ or BINARY_SEARCH_CLZ
#endif


#if !defined(BINARY_AND_CLZ) && !defined(BINARY_SHIFT_CLZ)
// #error #define one of BINARY_AND_CLZ or BINARY_SHIFT_CLZ
#define BINARY_AND_CLZ
#endif

#if defined(BINARY_AND_CLZ) && defined(BINARY_SHIFT_CLZ)
#error #define only one of BINARY_AND_CLZ or BINARY_SHIFT_CLZ
#endif

inline uint8_t clz(
uint32_t    bits)
{
#ifdef BUILTIN_CLZ
    if (bits == 0) return 32;
    else           return __builtin_clz(bits);
#endif

#ifdef BINARY_SEARCH_CLZ
    if (bits == 0) return 32;

    uint8_t     result = 0;

#ifdef BINARY_AND_CLZ
    if (!(bits & 0xFFFF0000)) { bits <<= 16; result += 16; }
    if (!(bits & 0xFF000000)) { bits <<=  8; result +=  8; }
    if (!(bits & 0xF0000000)) { bits <<=  4; result +=  4; }
    if (!(bits & 0xC0000000)) { bits <<=  2; result +=  2; }
    if (!(bits & 0x80000000)) {              result +=  1; }
#endif

#ifdef BINARY_SHIFT_CLZ
    if (!(bits >> 16)) { bits <<= 16; result += 16; }
    if (!(bits >> 24)) { bits <<=  8; result +=  8; }
    if (!(bits >> 28)) { bits <<=  4; result +=  4; }
    if (!(bits >> 30)) { bits <<=  2; result +=  2; }
    result = result - (bits >> 31);
#endif

    return result;
#endif
}



inline uint8_t highest_bit(
uint32_t    bits)
{
    uint8_t     result = 31;

    if (!(bits & 0xFFFF0000)) { bits <<= 16; result -= 16; }
    if (!(bits & 0xFF000000)) { bits <<=  8; result -=  8; }
    if (!(bits & 0xF0000000)) { bits <<=  4; result -=  4; }
    if (!(bits & 0xC0000000)) { bits <<=  2; result -=  2; }
    if (!(bits & 0x80000000)) {              result -=  1; }

    return result;
}


inline uint8_t num_lsbs(
uint32_t    bits)
{
    if (bits == 0) return 0;

#if 1
    uint8_t     result = 32;

    if (!(bits & 0xFFFF0000)) { bits <<= 16; result -= 16; }
    if (!(bits & 0xFF000000)) { bits <<=  8; result -=  8; }
    if (!(bits & 0xF0000000)) { bits <<=  4; result -=  4; }
    if (!(bits & 0xC0000000)) { bits <<=  2; result -=  2; }
    if (!(bits & 0x80000000)) {              result -=  1; }

    return result;
#else
    return highest_bit(bits) + 1;
#endif
}



constexpr inline uint32_t lsbs_mask(
const uint32_t  width)
{
#if 0
    if (width == 32) return 0xFFFFFFFF;
    return (1 << width) - 1;
#else
    return 0xFFFFFFFF >> (32 - width);
#endif
}

}   // namespace bitops

#endif   // ifndef BITOPS_HXX
