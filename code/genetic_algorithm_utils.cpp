#include "genetic_algorithm.h"
using namespace std;

bitset<32> genetic_algorithm::float_to_bitset(float input_float){
  data.input = input_float;
  bitset<32>   bits(data.output);
  return bits;
}

float genetic_algorithm::bitset_to_float(bitset<32> input_set){
  uint32_t const u = input_set.to_ullong();
  float d;

  // Aliases to `char*` are explicitly allowed in the Standard (and only them)
  char const* cu = reinterpret_cast<char const*>(&u);
  char* cd = reinterpret_cast<char*>(&d);

  // Copy the bitwise representation from u to input_set
  memcpy(cd, cu, sizeof(u));
  return d;
}

chromosome genetic_algorithm::get_chromosome( fann_connection* connections , unsigned int num_connections){
  chromosome new_chromo;

  for (int i = 0; i < num_connections; ++i)
    new_chromo.push_back( float_to_bitset(connections[i].weight) );

  return new_chromo;
}

chromosome genetic_algorithm::add_gaussian_noise_to_chromosome( chromosome input_chromo ){
  chromosome output_chromo;

  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = 1.0;
  double tmp = 0;
  std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
  std::normal_distribution<double> dist(mean, stddev);

  // Add Gaussian noise
  for (size_t i = 0; i < input_chromo.size(); i++) {
    tmp = bitset_to_float(input_chromo[i]);
    tmp += dist(generator);
    output_chromo.push_back( float_to_bitset(tmp) );
  }

  return output_chromo;
}

void genetic_algorithm::debug_stop(std::string action){
 std::cout << action << std::endl;
 std::cout << "Press ENTER to continue" << std::endl;
 std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
}

// TODO: THE FOLLOWING I ABLE TO RESIZE
// for (size_t i = 0; i < 8; i++)
//   super_population[0][0].reset(i);
//
// std::bitset<24> hello;
//
// for (size_t i = 31; i > 7; i--)
//   hello[i-8] = super_population[0][0][i];
//
// cout << endl;
// cout << hello << "\n" << super_population[0][0] << endl;
