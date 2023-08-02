#include <func/tables.hpp>
#pragma once
namespace lust {
extern const func::UniformChebyInterpTable<2,float> exp2lutf;
extern const func::UniformChebyInterpTable<4,double> exp2lutd;
extern const func::UniformLinearRawInterpTable<float> log2lutf;
extern const func::UniformChebyInterpTable<4,double> log2lutd;
}

