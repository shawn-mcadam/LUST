/* LUT of boost::math::expint(N,x) for N=1,2,3,4
 * Inverse: 
 * Derivative: 
 * */
#include <func/func.hpp>
#include <cmath>
#include <string>

#ifdef LUST_DEV
#include <boost/math/special_functions/expint.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#endif

namespace lust {

//template <typename T>
//static T f(unsigned int N, T x){
//  // need to handle the poly at x=0 when N == 1
//  return (N == 1) ? 1/(1+boost::math::expint(N,tan(pi*x/2))) : boost::math::expint(N,tan(pi*x/2));
//}

// TODO make this an array of filenames for each N we want to support
static std::string fname = "expintN2.json";

#ifdef LUST_DEV
template<typename T>
static T f(T x){
  if(x<=0)
    return 1;
  else
    return boost::math::expint(2, x);
}
static func::FunctionContainer<float> ffc {FUNC_SET_F(f,float)};
static func::FunctionContainer<double> dfc {FUNC_SET_F(f,double)};
static func::FunctionContainer<long double> ldfc {FUNC_SET_F(f,long double)};
#else
static func::FunctionContainer<float> ffc {nullptr};
static func::FunctionContainer<double> dfc {nullptr};
static func::FunctionContainer<long double> ldfc {nullptr};
#endif

static long double left = 1.0;
static long double right = 2.0;
static func::LookupTableGenerator<float,float,double> fgen(&ffc, left, right);
static func::LookupTableGenerator<double,double,boost::multiprecision::cpp_bin_float_quad> dgen(&dfc, left, right);
static func::LookupTableGenerator<long double,long double,boost::multiprecision::cpp_bin_float_oct> ldgen(&ldfc, left, right);

// TODO build an array of LUTs
#ifdef LUST_DEV
static const auto flut = fgen.generate_by_tol(
    "NonUniformPseudoLinearPrecomputedInterpolationTable",
    1e2*std::numeric_limits<float>::epsilon(),
    "f" + fname);
static const auto dlut = dgen.generate_by_tol(
    "UniformArmadilloPrecomputedInterpolationTable<7>",
    1e5*std::numeric_limits<double>::epsilon(),
    "d" + fname);
static const auto ldlut = ldgen.generate_by_tol( // woefully slow...
    "UniformArmadilloPrecomputedInterpolationTable<7>",
    1e11*std::numeric_limits<long double>::epsilon(),
    "ld" + fname);
#else
//static const auto flut = fgen.generate_by_file("f"+fname);
//static const auto dlut = dgen.generate_by_file("d"+fname);
//static const auto ldlut = ldgen.generate_by_file("ld"+fname);
#endif

float expint(float x){
  return (*flut)(x);
}

double expint(double x){
  return (*dlut)(x);
}

long double expint(long double x){
  return (*ldlut)(x);
}

} // namespace lust
