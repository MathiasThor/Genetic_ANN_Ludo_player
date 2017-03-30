#include "super_ludo_player.h"

using namespace std;

bitset<32> super_ludo_player::float_to_bitset(float input_float){
  data.input = input_float;
  bitset<32>   bits(data.output);
  return bits;
}

float super_ludo_player::bitset_to_float(bitset<32> input_set){
  uint32_t const u = input_set.to_ullong();
  float d;

  // Aliases to `char*` are explicitly allowed in the Standard (and only them)
  char const* cu = reinterpret_cast<char const*>(&u);
  char* cd = reinterpret_cast<char*>(&d);

  // Copy the bitwise representation from u to input_set
  memcpy(cd, cu, sizeof(u));
  return d;
}

void super_ludo_player::set_chromosome_as_weights( chromosome input_chromo ){
  unsigned int num_connections = net.get_total_connections();
  struct fann_connection *connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  for (int i = 0; i < num_connections; ++i)
    connections[i].weight = bitset_to_float(input_chromo[i]);

  net.set_weight_array(connections, num_connections);
}

chromosome super_ludo_player::get_chromosome( fann_connection* connections , unsigned int num_connections){
  chromosome new_chromo;

  for (int i = 0; i < num_connections; ++i)
    new_chromo.push_back( float_to_bitset(connections[i].weight) );

  return new_chromo;
}

chromosome super_ludo_player::add_gaussian_noise_to_chromosome( chromosome input_chromo ){
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

void super_ludo_player::debug_stop(std::string action, int pos, bool cout_positions){
 std::cout << "Can " << action << " with DiceRoll: " << dice_roll << std::endl;
 std::cout << "Current Player pos: " << pos << std::endl;
 if (cout_positions) {
   for (int i = 0; i < pos_start_of_turn.size(); i++) {
     std::cout << "Debug_pos " << i << ": " << pos_start_of_turn[i] << std::endl;
   }
 }
 std::cout << "Press ENTER to continue" << std::endl;
 std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
}

std::vector < int > super_ludo_player::sorted_index(fann_type* calc_out){
  double fourth_largest = 0;
  double third_largest = 0;
  double second_largest = 0;
  double largest = 0;
  int fourth_largest_index = -1;
  int third_largest_index = -1;
  int second_largest_index = -1;
  int largest_index = -1;
  //Finding Largest
  for (int i = 0; i < 4; ++i)
    if (calc_out[i]>largest){
      largest = calc_out[i];
      largest_index = i;
    }
  //finding second largset
  for (int i = 0; i < 4; ++i)
   if (calc_out[i]>second_largest){
      if (calc_out[i] == largest)
            continue;
      second_largest = calc_out[i];
      second_largest_index = i;
   }
  //finding third largset
  for (int i = 0; i < 4; ++i)
   if (calc_out[i]>third_largest){
      if (calc_out[i] == largest || calc_out[i] == second_largest)
            continue;
      third_largest = calc_out[i];
      third_largest_index = i;
   }
  //finding fourth largset
  for (int i = 0; i < 4; ++i)
   if (calc_out[i]>third_largest){
      if (calc_out[i] == largest || calc_out[i] == second_largest || calc_out[i] == third_largest)
            continue;
      fourth_largest = calc_out[i];
      fourth_largest_index = i;
   }

   return vector<int>{largest_index,second_largest_index,third_largest_index,fourth_largest_index};
}
