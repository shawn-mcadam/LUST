#include <func/func.hpp>
#include <cmath>

#define LUST_DENORMALS
#include "lust.hpp"

//#define TYPE float
#define TYPE double

int main(){
  unsigned int nExperiments = 200;
  unsigned int nEvals       = 1000000;
  unsigned int seed         = 0;

  std::vector<std::tuple<std::string,func::FunctionContainer<TYPE>,func::FunctionContainer<TYPE>,TYPE,TYPE>> functions{
    {"log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 1.0e-8, 2},
    {"subnormal float log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 1.4014e-45f, 1.17548e-38f},
    {"float exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -125, 126},
    //{"subnormal float exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -149, -127},
    //{"subnormal double log2", {[](TYPE x){ return std::log2(x); }}, {[](TYPE x){ return lust::log2(x); }}, 4.94065e-324, 2.22507e-308},
    //{"double exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -1021, 1022},
    //{"subnormal double exp2", {[](TYPE x){ return std::exp2(x); }}, {[](TYPE x){ return lust::exp2(x); }}, -1074, -1023},
    {"pow", {[](TYPE x){ return std::pow((TYPE)3.1415,x); }}, {[](TYPE x){ return lust::pow((TYPE)3.1415,x); }}, -10.0, 10.0},
    {"pow", {[](TYPE x){ return std::pow((TYPE)3.1415,x); }}, {[](TYPE x){ return lust::pow((TYPE)3.1415,x); }}, 0.0, 1.0},
    {"pow", {[](TYPE x){ return std::pow((TYPE)3.1415,x); }}, {[](TYPE x){ return lust::pow((TYPE)3.1415,x); }}, -1.0, 0.0},
  };

  for(auto name_fc1_fc2_min_max : functions){
    std::string name; func::FunctionContainer<TYPE> fc1, fc2; TYPE min, max;
    std::tie(name, fc1, fc2, min, max) = name_fc1_fc2_min_max;

    std::vector<std::unique_ptr<func::LookupTable<TYPE>>> luts;
    luts.emplace_back(std::unique_ptr<func::LookupTable<TYPE>>(new func::DirectEvaluation<TYPE>(fc1)));
    luts.emplace_back(std::unique_ptr<func::LookupTable<TYPE>>(new func::DirectEvaluation<TYPE>(fc2)));
    std::cerr << "Running timings" << std::endl;

    /* Run comparator */
    func::LookupTableComparator<TYPE> implCompare(luts, min, max, nEvals, seed);
    implCompare.run_timings(nExperiments);

    /* Summarize the results */
    std::cout << name << std::endl;
    std::cout << "(" << min << "," << max << ")" << std::endl;

    implCompare.compute_statistics();
    implCompare.print_summary(std::cout);
  }
}
