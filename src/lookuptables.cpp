#include "lookuptables.hpp"
#include "lookuptablejsons.hpp"

namespace lust {
const func::UniformChebyInterpTable<2,float> exp2lutf({},{},exp2lutf_json);
const func::UniformChebyInterpTable<4,double> exp2lutd({},{},exp2lutd_json);
const func::UniformLinearRawInterpTable<float> log2lutf({},{},log2lutf_json);
const func::UniformChebyInterpTable<4,double> log2lutd({},{},log2lutd_json);
}

