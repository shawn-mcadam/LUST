#include <boost/math/special_functions/expint.hpp>
#include <func/func.hpp>

// TODO does this automatically work with Boost's automatic differentiation?
// Can the choice in LUT be determined by the choice of T?
namespace lust {
//template <unsigned int N, typename T, T a, T b, T tol>
//         //class LUT_TYPE = LinearInterpolationTable<T>>
//class expint
//{
//  // can we build a different function if people try adding/dividing this class by something else?
//  template <typename T2>
//  T2 f(T2 z){
//    return boost::math::expint(N,z);
//  }
//
//  inline static const func::FunctionContainer<T> fc {FUNC_SET_F(f, T)};
//  inline static const func::LookupTableGenerator<T> gen(&fc, a, b);
//  // make a LUT out of the error at stepsize h?
//  // could make classname member variable public const for every LUT????
//  inline static const func::FailureProofTable<T> m_lut(gen.generate_by_tol("UniformLinearInterpolationTable", tol));
//public:
//  expint(T x){}
//
//  T operator()(T x){
//    // build the LUT the first time this is called?
//    return m_lut(x);
//  }
//};

static const func::FunctionContainer<T> fc {FUNC_SET_F(f, T)};
static const func::LookupTableGenerator<T> gen(&fc, a, b);
static const func::FailureProofTable<T> m_lut(gen.generate_by_tol("UniformLinearInterpolationTable", tol));

[]<typename T>(T&& value){ std::cout << value << '\n'; };
}

