#define LUST_DEV
#include "expint.hpp"

int main(){
  lust::generate_expint();
  std::cout << lust::expint(1.0) << std::endl;
}
