#include <boost/math/special_functions/expint.hpp>
#include <func/func.hpp>

// TODO does this automatically work with Boost's automatic differentiation?
// Can the choice in LUT be determined by the choice of T?
namespace lust {
template <unsigned int N, typename T, T a, T b, T step>
         //class LUT_TYPE = LinearInterpolationTable<T>>
class expint
{
  // can we build a different function if people try adding/dividing this class by something else?
  template <typename T2>
  static T2 f(T2 z){
    return boost::math::expint(N,z);
  }

  inline static auto fc = func::FunctionContainer<T>(FUNC_SET_F(f, T));
  // for some reason generate_by_tol causes the program to hang...
  inline static func::FailureProofTable<T> m_lut = func::LookupTableGenerator<T>(&fc,a,b).generate_by_step("UniformCubicPrecomputedInterpolationTable", step);
public:
  expint() = default;

  T operator()(T x){
    // build the LUT the first time this is called?
    return m_lut(x);
  }
};

//static const func::FunctionContainer<T> fc {FUNC_SET_F(f, T)};
//static const func::LookupTableGenerator<T> gen(&fc, a, b);
//static const func::FailureProofTable<T> m_lut(gen.generate_by_tol("UniformLinearInterpolationTable", tol));
//
//[fc = func::FunctionContainer<T>(FUNC_SET_F(f, T)), func::FailureProofTable<T>(func::LookupTableGenerator<T>(&fc, a, b).generate_by_tol())]<typename T>(T&& value){ std::cout << value << '\n'; };
}

