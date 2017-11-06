#include <iostream>
#include "getopt.h"
#include "Approximator.h"

void PrintHelp() {
  std::cout << "Help information: " << std::endl;
  std::cout << "  -i path/to/input_file.bmp --> Directory of target image in bmp format" << std::endl;
  std::cout << "  -o path/to/output_file.bmp--> Directory of output image in bmp format" << std::endl;
  std::cout << "  -g bool(1,0)              --> Outputs a sequence of images to create a Gif. Default is 0" << std::endl;
  std::cout << "  -t n_threads              --> Number of threads to be used. Default is 4" << std::endl;
  std::cout << "  -s n_random_states        --> Number of random states generated. Default is 1000" << std::endl;
  std::cout << "  -c n_hill_climbs          --> Number of Hill Climb steps. Default is 100" << std::endl;
  std::cout << "  -m max_hill_climbs        --> Maximum number of Hill Climb evaluations. Default is 1000" << std::endl;
  std::cout << "  -r max_random             --> Maximum random used for generations/mutations. Default is 16" << std::endl;
  std::cout << "  -a alpha                  --> Alpha value used for geometric primitives. Default is 128" << std::endl;
  std::cout << "  -n n_primitives           --> Number of geometric primitives used. Default is 250" << std::endl;
  std::cout << "  -p primtive_id            --> Geometric primitive used. 0 - Triangles, 1 - Ellipses. Default is 0" << std::endl;

}


int main(int argc, char **argv) {
  int n_threads       = 4,
      n_random_states = 1000,
      n_climbs        = 100,
      max_climbs      = 1000,
      max_random      = 16,
      alpha           = 128,
      n_shapes        = 250;

  bool gif_flag       = false;
  ShapeType shape     = ShapeType::ELLIPSE;
  std::string input   = "",
              output  = "";

  int c;
  while ((c = getopt(argc, argv, "i:o:t:s:c:m:r:a:n:p:g:")) != -1) {
    switch (c){
    case 'g':
      gif_flag = atoi(optarg);
      break;
    case 'i':
      input = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    case 't':
      n_threads = atoi(optarg);
      break;
    case 's':
      n_random_states = atoi(optarg);
      break;
    case 'c':
      n_climbs = atoi(optarg);
      break;
    case 'm':
      max_climbs = atoi(optarg);
      break;
    case 'r':
      max_random = atoi(optarg);
      break;
    case 'a':
      alpha = atoi(optarg);
      break;
    case 'n':
      n_shapes = atoi(optarg);
      break;
    case 'p':
      switch (atoi(optarg)) {
      case 0:
        shape = ShapeType::TRIANGLE;
        break;
      case 1:
        shape = ShapeType::ELLIPSE;
        break;
      default:
        PrintHelp();
        abort();
      }
      break;
    default:
      PrintHelp();
      abort();
    }
  }

  if (input.length() == 0) {
    std::cerr << "Error: -i parameter should be provided" << std::endl;
    abort();
  }

  if (output.length() == 0) {
    std::cerr << "Error: -o parameter should be provided" << std::endl;
    abort();
  }

  std::cout << "Parameters: " << std::endl;
  std::cout << "\t-i: " << input << std::endl;
  std::cout << "\t-o: " << output << std::endl;
  std::cout << "\t-g: " << gif_flag << std::endl;
  std::cout << "\t-t: " << n_threads << std::endl;
  std::cout << "\t-s: " << n_random_states << std::endl;
  std::cout << "\t-c: " << n_climbs << std::endl;
  std::cout << "\t-m  " << max_climbs << std::endl;
  std::cout << "\t-r  " << max_random << std::endl;
  std::cout << "\t-a  " << alpha << std::endl;
  std::cout << "\t-n  " << n_shapes << std::endl;


  const Image target = Image(input);
  if (target.GetSize() == 1) {
    abort();
  }

  Approximator approximator(target, n_threads);
  approximator.OUTPUT_PATH = output;
  approximator.N_RANDOM_STATE = n_random_states;
  approximator.N_CLIMBS = n_climbs;
  approximator.MAX_CLIMBS = max_climbs;
  approximator.MAX_RANDOM = max_random;
  approximator.ALPHA = alpha;
  approximator.N_SHAPES = n_shapes;
  approximator.GIF = gif_flag;
  approximator.SHAPE_TYPE = shape;

  approximator.Run();
  
  return 0;
}