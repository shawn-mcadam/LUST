#include <func/tables.hpp> // includes nlohmann's json.hpp
#pragma once
namespace lust {
namespace detail {
extern const func::UniformChebyInterpTable<2,float>  exp2lutf;
extern const func::UniformChebyInterpTable<4,double> exp2lutd;
extern const func::UniformLinearRawInterpTable<float> log2lutf;
extern const func::UniformChebyInterpTable<4,double>  log2lutd;

//const func::UniformChebyInterpTable<2,float>   exp2lutf({[](float  x){ return std::exp2(x); }}, { -1,1,0.0625});
//const func::UniformChebyInterpTable<4,double>  exp2lutd({[](double x){ return std::exp2(x); }}, { -1,1,0.0195108});
//const func::UniformLinearRawInterpTable<float> log2lutf({[](float  x){ return std::log2(x); }}, {0.5,1,0.0009765619761310518});
//const func::UniformChebyInterpTable<4,double>  log2lutd({[](double x){ return std::log2(x); }}, {0.5,1,0.00354267});
}
}

