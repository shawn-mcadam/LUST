/* Approximate boost::math::expint(N,x) for N=1,2,3,4
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
  return boost::math::expint(2, x);
}
static func::FunctionContainer<float> ffc {f<float>};
static func::FunctionContainer<double> dfc {f<double>};
//static func::FunctionContainer<long double> ldfc {f<long double>};
#else
static func::FunctionContainer<float> ffc {nullptr};
static func::FunctionContainer<double> dfc {nullptr};
//static func::FunctionContainer<long double> ldfc {nullptr};
#endif

static func::LookupTableGenerator<float,float,double> fgen(&ffc, 0, 1);
static func::LookupTableGenerator<double,double,boost::multiprecision::cpp_bin_float_quad> dgen(&dfc, 0, 1);
//static func::LookupTableGenerator<long double,long double,boost::multiprecision::cpp_bin_float_oct> ldgen(&ldfc, 0, 1);

#ifdef LUST_DEV
void generate_expint(){
  //for(unsigned int N=1; N<=4; N++){
  //N = 2;
  //std::string fname = "expintN" + std::to_string(N) + ".json";
  fgen.generate_by_tol(
      "PseudoLinearPrecomputedInterpolationTable",
      2*std::numeric_limits<float>::epsilon(),
      "f" + fname);
  dgen.generate_by_tol("CubicPrecomputedInterpolationTable",
      2*std::numeric_limits<double>::epsilon(),
      "d" + fname);
  //ldgen.generate_by_tol("ArmadilloPrecomputedInterpolationTable<7>",
  //    2*std::numeric_limits<long double>::epsilon(),
  //    "ld" + fname);
  //}
}
#endif

// TODO ideally we only read the file when the function is called the first time but idk if that's possible
// TODO build an array of LUTs
static const auto flut = fgen.generate_by_file("f"+fname);
static const auto dlut = dgen.generate_by_file("d"+fname);
//static const auto ldlut = ldgen.generate_by_file("ld"+fname);

float expint(float x){
  return (*flut)(x);
}

double expint(double x){
  return (*dlut)(x);
}

//long double expint(long double x){
//  return (ldlut*)(x);
//}

} // namespace lust
