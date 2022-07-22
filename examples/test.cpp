#include "expint.hpp"

int main(){
  std::cout << "foo\n";
  auto lut = lust::expint<1, double, 1e-8, 1.0, 1e-2>();
  std::cout << lut(1) << std::endl;
  //std::cout << lust::expint<1, double, 1e-8, 1.0, 1e-2>()(1) << std::endl;
  std::cout << "bar\n";
}
