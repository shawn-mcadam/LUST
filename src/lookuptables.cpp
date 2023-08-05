#include "lookuptables.hpp"
#include "lookuptablejsons.hpp"

/* hide maybe uninitialized warnings by setting each LookupTableParameters to {0,0,0} instead of {} */

namespace lust {

/* -- exp2 lookuptables -- */
const func::UniformChebyInterpTable<2,float>  exp2lutf({},{0,0,0},exp2lutf_json);
const func::UniformChebyInterpTable<4,double> exp2lutd({},{0,0,0},exp2lutd_json);

/* -- log2 lookuptables -- */
const func::UniformLinearRawInterpTable<float> log2lutf({},{0,0,0},log2lutf_json);
const func::UniformChebyInterpTable<4,double>  log2lutd({},{0,0,0},log2lutd_json);
}

