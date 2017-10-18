#include <iostream>
#include "Image.h"
#include "Approximator.h"
#include <math.h>

int main() {
  // Initialization
  Image target = Image("../assets/monalisa.bmp");
  
  // Approximate Image using Hill Climber algorithm
  Image current = Approximator(target).Run();

  // Write output
  current.Export("output.bmp");

  return 0;
}

