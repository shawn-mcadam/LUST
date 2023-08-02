#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <tuple>

#define LUST_DENORMALS
#include "lust.hpp"

#define TYPE float

int main(){
  //TYPE N = 10004439.0;
  //TYPE N = 5004439.0;
  TYPE N = 504439.0;
  //TYPE N = 100.0;
  //TYPE N = 2.0;
  //TYPE N = 531.0;

  std::vector<std::tuple<std::string,std::function<TYPE(TYPE)>,std::function<TYPE(TYPE)>,TYPE,TYPE>> functions {
    {"float log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 1.17548e-38, 4},
    {"subnormal float log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 1.4014e-45f, 1.17548e-38f},
    {"float exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -125, 126},
    {"subnormal float exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -149, -126},
    //{"double log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 2.22507e-307, 4},
    //{"subnormal double log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 4.94065e-324, 2.22507e-308},
    //{"double exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -1021, 1022},
    //{"subnormal double exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -1074, -1022},
    {"pow", {[](TYPE x){ return std::pow((TYPE)3.1415,x); }}, {[](TYPE x){ return lust::pow((TYPE)3.1415,x); }}, -10.0, 10.0},
  };

  for(auto name_f1_f2_min_max : functions){
    std::string name; std::function<TYPE(TYPE)> f1, f2; TYPE min, max;
    std::tie(name, f1, f2, min, max) = name_f1_f2_min_max;
    auto f = [f1,f2](TYPE x){ return std::abs(f1(x)-f2(x))/(1.0+std::abs(f1(x))); };
    //auto f = [f1,f2](TYPE x){ auto f1x = f1(x); auto f2x = f2(x); return 2*std::fabs(f1x-f2x)/(std::fabs(f1x) + std::fabs(f2x)); };
    
    auto maxerr = 0.0; auto maxx = 0.0;
    for(TYPE x = min; x <= max; x += (max-min)/N){
      auto y = f(x);
      if(y > maxerr){
        maxerr = y;
        maxx = x;
      }
    }
    std::cout << name << " has max error of " << maxerr << " at x=" << maxx << "." << std::endl;
  }
}
