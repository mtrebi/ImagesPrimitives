#include <iostream>
#include "Approximator.h"

int main() {
  // Initialization
  Image target = Image("../assets/monalisa.bmp");
  Approximator approximator(target);
  approximator.Run();

  return 0;
}