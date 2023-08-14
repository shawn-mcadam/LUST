#pragma once

#if __cplusplus >= 202002L
  #define LUST_LIKELY [[likely]]
  #define LUST_UNLIKELY [[unlikely]]
  #define LUST_BITCAST std::bit_cast
#else
  #define LUST_LIKELY
  #define LUST_UNLIKELY
  #define LUST_BITCAST lust::detail::bit_cast
#endif

