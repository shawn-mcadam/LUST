#include "lookuptables.hpp"
#include "config.hpp"

#pragma once

namespace lust {
namespace detail {
template <class To, class From>
static inline To bit_cast(const From& src) noexcept {
  static_assert(sizeof(To) == sizeof(From), "lust::detail::bit_cast cannot cast types of unequal size but sizeof(To) != sizeof(From)");
  static_assert(std::is_trivially_copyable<From>::value, "input type of lust::bit_cast must be trivially copyable but it is not");
  static_assert(std::is_trivially_copyable<To>::value,   "output type of lust::bit_cast must be trivially copyable but it is not");
  static_assert(std::is_trivially_constructible<To>::value,
        "This implementation additionally requires "
        "destination type to be trivially constructible");

  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}
}
}

namespace lust {

/* Compute 2^x as (1 << floor(x))*2^fractional(x)
 * - Uses properties of floating point numbers. exponent(2^x) = floor(x) and we can use a LUT of 2^x over [-1,1].
 * - This works if the output is subnormal; however, the result does not necessarily underflow exactly when
 *   std::exp2 would... This will never be an issue.
 * - max error for normalized numbers: 5.51549e-07 */
static inline float exp2(const float x){
  const auto exponent = static_cast<int32_t>(x);
  auto fractional = exp2lutf(x-exponent);

#ifndef LUST_DENORMALS
  uint32_t result_bits = detail::bit_cast<uint32_t>(fractional) + (exponent << 23);
#else
  uint32_t result_bits;
  constexpr auto minexp = -126;
  if (x >= minexp) LUST_LIKELY {
    result_bits = detail::bit_cast<uint32_t>(fractional) + (exponent << 23);
  } else {
    /* the result is a subnormal number. Recall, subnormal numbers are fixed point,
     * their exponent field is 0 and they have an explicit leading "1" */
    constexpr auto mantissa_bits = UINT32_C(0x007FFFFF);
    constexpr auto leading_one = UINT32_C(0x00800000);
    result_bits = (detail::bit_cast<uint32_t>(fractional) & mantissa_bits) | leading_one;

    /* All that's left is to bitshift and correctly round the result.
     * offset handles a special case where x is a whole number (very unlikely...) */
    const auto offset = (x != exponent) ? 1 : 0;
    const auto shift  = minexp + offset - exponent;
    const auto round  = (result_bits & (1 << (shift - 1))) ? 1 : 0;
    result_bits = (result_bits >> shift) + round;
  }
#endif
  return detail::bit_cast<float>(result_bits);
}

/* max error for normalized numbers: 7.8102e-15 */
static inline double exp2(const double x){
  const auto exponent = static_cast<int64_t>(x);
  auto fractional = exp2lutd(x-exponent);

#ifndef LUST_DENORMALS
  uint64_t result_bits = detail::bit_cast<uint64_t>(fractional) + (exponent << 52);
#else
  uint64_t result_bits;
  constexpr auto minexp = -1022;
  if (x >= minexp) LUST_LIKELY {
    result_bits = detail::bit_cast<uint64_t>(fractional) + (exponent << 52);
  }else{
    constexpr auto mantissa_bits = UINT64_C(0x000FFFFFFFFFFFFF);
    constexpr auto leading_one = UINT64_C(0x0010000000000000);
    result_bits = (detail::bit_cast<uint64_t>(fractional) & mantissa_bits) | leading_one;

    const auto offset = (x == exponent) ? 0 : 1;
    const auto round  = (result_bits & (1 << (minexp + offset - exponent - 1))) ? 1 : 0;
    result_bits = (result_bits >> (minexp + offset - exponent)) + round;
  }
#endif
  return detail::bit_cast<double>(result_bits);
}

/* Return a pair containing (exponent, mantissa) of the floating point representation of x.
 * mantissa is a number in [0.5, 1] and exponent == static_cast<int>(log2(x)). */
static inline std::pair<int32_t,float> frexp(float x){
  /* 126 is the exponent bias and 23 is 1-(num float mantissa bits) */
  constexpr uint32_t mantissa_bits = UINT32_C(0x007FFFFF); /* 0x007FFFFF == 2^23-1 */
  constexpr uint32_t bias_bits     = UINT32_C(0x3F000000); /* 0x3F000000 == (126ull << 23) */
  constexpr uint32_t no_sign       = UINT32_C(0x7FFFFFFF); /* 0x7FFFFFFF == 2^32-1 */
  int32_t absx_bits = (detail::bit_cast<int32_t>(x) & no_sign);

#ifdef LUST_DENORMALS
  if(detail::bit_cast<float>(absx_bits) < FLT_MIN) LUST_UNLIKELY {
    int32_t exponent; float mantissa;
    std::tie(exponent, mantissa) = frexp(x*(UINT32_C(1) << 23));
    return std::make_pair(exponent - 23, mantissa);
  }
#endif

  /* get the mantissa bits of x with the exponent set to bias */
  int32_t  exponent = (absx_bits >> 23) - 126;
  uint32_t mantissa = (detail::bit_cast<uint32_t>(x) & mantissa_bits) | bias_bits;
  return std::make_pair(exponent, detail::bit_cast<float>(mantissa));
}

static inline std::pair<int64_t,double> frexp(double x){
  /* 1022 is the exponent bias and 52 is 1-(num double mantissa bits) for double */
  constexpr uint64_t mantissa_bits = UINT64_C(0x000FFFFFFFFFFFFF); /* 0x000FFFFFFFFFFFFF == 2^52-1 */
  constexpr uint64_t bias_bits     = UINT64_C(0x3FE0000000000000); /* 0x3FE0000000000000 == (1022ull << 52) */
  constexpr uint64_t no_sign       = UINT64_C(0x7FFFFFFFFFFFFFFF); /* 0x7FFFFFFFFFFFFFFF == 2^64-1 */
  int64_t absx_bits = (detail::bit_cast<int64_t>(x) & no_sign);

#ifdef LUST_DENORMALS
  if(detail::bit_cast<double>(absx_bits) < DBL_MIN) LUST_UNLIKELY {
    int64_t exponent; double mantissa;
    std::tie(exponent, mantissa) = frexp(x*(UINT64_C(1) << 52));
    return std::make_pair(exponent - 52, mantissa);
  }
#endif

  /* grab all the mantissa bits from x and add the exponent bias to the result */
  int64_t  exponent = (absx_bits >> 52) - 1022;
  uint64_t mantissa = (detail::bit_cast<uint64_t>(x) & mantissa_bits) | bias_bits;
  return std::make_pair(exponent, detail::bit_cast<double>(mantissa));
}

/* max error for normalized numbers: 7.14416e-07 */
static inline float log2(const float x){
  int32_t exponent; float mantissa;
  std::tie(exponent, mantissa) = frexp(x);
  return log2lutf(mantissa) + exponent;
  //return apply(log2lutf,mantissa) + exponent;
}

/* max error for normalized numbers: 1.01926e-14 */
static inline double log2(const double x){
  int64_t exponent; double mantissa;
  std::tie(exponent, mantissa) = frexp(x);
  return log2lutd(mantissa) + exponent;
}

/* Error of the following logarithms are proportional to the error of log2 */
#define INVLOG2OF10 \
0.3010299956\
639811952137\
38894724493l
#define INVLOG2OFe \
0.6931471805\
599453094172\
321214581766l

static inline float log10(const float x){
  return INVLOG2OF10*log2(x);
}

static inline double log10(const double x){
  return INVLOG2OF10*log2(x);
}

static inline float log(const float x){
  return INVLOG2OFe*log2(x);
}

static inline double log(const double x){
  return INVLOG2OFe*log2(x);
}


/* Compute x^p = 2^{p log_2(x)}
 * Approximating log2 over [0.5,1] with a degree 7 equiosciallating polynomial (computed with lolremez).
 * - Evaluated via Horner's method.
 * - This approximation of log2 over [0.5,1] performs better than lust::log2 for computing pow
 * - max error for normalized numbers: 8.76507e-06 */
static inline float pow(const float x, const float p){
  int32_t exponent; float man;
  std::tie(exponent, man) = frexp(x);
  float log2x = ((((((1.936046f*man-11.772137f)*man+31.299035f)*man-47.760338f)*man+46.251513f)*man-29.976521f)*man+14.26794f)*man-4.2455379f + exponent;

  return exp2(p * log2x);
}

/* max error for normalized numbers: 2.20904e-12 */
static inline double pow(const double x, const double p){
  int64_t exponent; double man;
  std::tie(exponent, man) = frexp(x);
  double u = 11.418139608138997;
  u = u * man + -137.13543914542672;
  u = u * man + 769.67346436467778;
  u = u * man + -2679.9204845738036;
  u = u * man + 6480.4300050748252;
  u = u * man + -11542.955241415944;
  u = u * man + 15672.298240301325;
  u = u * man + -16555.327579441491;
  u = u * man + 13763.369333621975;
  u = u * man + -9048.9592467476323;
  u = u * man + 4701.8645114052715;
  u = u * man + -1920.2430007059663;
  u = u * man + 610.97856754993245;
  u = u * man + -150.79824189558093;
  u = u * man + 30.596730326941152;
  double log2x = u * man + -5.2897583272396238 + exponent;

  return exp2(p * log2x);
}

}
