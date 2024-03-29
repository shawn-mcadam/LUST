#include <cmath>
#include <func/func.hpp>
#include <boost/math/differentiation/autodiff.hpp>
#include <filesystem> /* requires c++17 */


namespace lust {

template <typename REAL_TYPE, size_t ORDER>
using fvar = boost::math::differentiation::autodiff_v1::detail::fvar<REAL_TYPE,ORDER>;

template <typename REAL_TYPE, size_t ORDER>
fvar<REAL_TYPE,ORDER> log2(fvar<REAL_TYPE,ORDER> const& cr){
  using boost::math::differentiation::make_fvar;
  // define the 0th derivative
  REAL_TYPE const d0 = std::log2(static_cast<REAL_TYPE>(cr));

  if(ORDER == 0)
    return d0;
  else{
    // Give Boost the first derivative and let it do the rest of the work
    auto x = make_fvar<REAL_TYPE, ORDER ? ORDER - 1 : 0>(static_cast<REAL_TYPE>(cr));
    auto const d1 = 1/log(2.0)/x; // d1 has the remaining ORDER-1 derivatives we need

    // define our function's taylor series expansion
    return cr.apply_coefficients(ORDER, [&d0, &d1](size_t i) { return i ? d1[i - 1] / i : d0; });
  }
}

template <typename REAL_TYPE, size_t ORDER>
fvar<REAL_TYPE,ORDER> exp2(fvar<REAL_TYPE,ORDER> const& cr){
  return exp(cr*log(2));
}

/* define each LUT */
template <typename T>
T templated_exp2(T x){ using std::exp2; using lust::exp2; return exp2(x); }
template <typename T>
T templated_log2(T x){ using std::log2; using lust::log2; return log2(x); }

}

// UniformChebyInterpTable<2> a=-1 b=1 h=0.0625 N=32 Memory usage 528 Bytes atol=rtol=5e-7
static func::UniformChebyInterpTable<2,float> exp2lutf({FUNC_SET_F(lust::templated_exp2, float)}, {-1,1,0.0625});
// UniformChebyInterpTable<4> a=-1 b=1 h=0.0194832 N=103 Memory usage: 6656 Bytes
static func::UniformChebyInterpTable<4,double> exp2lutd({FUNC_SET_F(lust::templated_exp2,double)}, {-1.0,1.0,0.0195108});
// UniformLinearRawInterpTable 0.5 1 0.000976562 512 Memory usage (B): 2056
static const func::UniformLinearRawInterpTable<float> log2lutf({FUNC_SET_F(lust::templated_log2,float)}, {0.5,1.0,0.000976562});
// UniformChebyInterpTable<4> 0.5 1 0.00354267 142 Memory usage (B): 9152
static func::UniformChebyInterpTable<4,double> log2lutd({FUNC_SET_F(lust::templated_log2,double)}, {0.5,1.0,0.00354267});



int main(){
  std::string filename = "lustjsons.hpp";
  if(std::filesystem::exists(filename))
    std::filesystem::remove(filename);

  std::ofstream filestreamer;
  filestreamer.open(filename);
  filestreamer << "/* This file is automatically generated by lustdev.cpp. This should only be included by luts.cpp."
    << "Do not edit this file manually. */";
  filestreamer << "#pragma once\nnamespace lust {\n";

#define PRINT_LUT_CODE(lutname) \
  do { \
    filestreamer << "static nlohmann::json " #lutname "_json = nlohmann::json::parse(R\"(\n"; \
    lutname.print_json(filestreamer); \
    filestreamer << ")\");\n"; \
  } while(0)

  PRINT_LUT_CODE(exp2lutf);
  PRINT_LUT_CODE(exp2lutd);
  PRINT_LUT_CODE(log2lutf);
  PRINT_LUT_CODE(log2lutd);

  filestreamer << "}\n";

  filestreamer.close();
}

