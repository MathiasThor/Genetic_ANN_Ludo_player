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
    // new_chromo.push_back( float_to_bitset(0) ); // Use for randomly initialized weights

  return new_chromo;
}

chromosome genetic_algorithm::add_gaussian_noise_to_chromosome( chromosome input_chromo ){
  chromosome output_chromo;

  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = GAUSSIAN_STDDEV;
  float tmp = 0;
  std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
  std::normal_distribution<float> dist(mean, stddev);

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

void genetic_algorithm::save_generation(population pop_to_save, string filename){
  std::ofstream file(filename, std::ofstream::binary);
  unsigned long n;
  for(size_t i = 0; i < pop_to_save.size(); i++ ) {
    for (size_t j = 0; j < pop_to_save[i].size(); j++) {
      n = pop_to_save[i][j].to_ulong();
      file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    }
  }
  file.close();
  std::cout << "Saved population to: " << filename << '\n';
}

void genetic_algorithm::save_chromosome(chromosome chrom_to_save, string filename){
  std::ofstream file(filename, std::ofstream::binary);
  unsigned long n;
  for (size_t j = 0; j < chrom_to_save.size(); j++) {
    n = chrom_to_save[j].to_ulong();
    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
  }
  file.close();
  std::cout << "Saved chromosome to: " << filename << '\n';
}

population genetic_algorithm::load_generation(string filename){
  std::ifstream file(filename, std::ofstream::binary);
  population loaded_pop(POP_SIZE, std::vector<bitset<32>> (394,0));
  unsigned long n;
  for(size_t i = 0; i < POP_SIZE; i++ ) {
    for (size_t j = 0; j < 394; j++) {
      file.read( reinterpret_cast<char*>(&n), sizeof(n) );
      loaded_pop[i][j] = n;
    }
  }
  file.close();
  std::cout << "Loaded population from: " << filename << '\n';
  return loaded_pop;
}

chromosome genetic_algorithm::load_chromosome(string filename){
  std::ifstream file(filename, std::ofstream::binary);
  chromosome loaded_chrom(394,0);
  unsigned long n;
  for (size_t j = 0; j < 394; j++) {
    file.read( reinterpret_cast<char*>(&n), sizeof(n) );
    loaded_chrom[j] = n;
  }
  file.close();
  std::cout << "Loaded chromosome from: " << filename << '\n';
  return loaded_chrom;
}

void genetic_algorithm::set_chromosome_as_weights( chromosome input_chromo ){
  unsigned int num_connections = net.get_total_connections();
  struct fann_connection *connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  for (int i = 0; i < num_connections; ++i)
    connections[i].weight = bitset_to_float(input_chromo[i]);

  net.set_weight_array(connections, num_connections);
}
