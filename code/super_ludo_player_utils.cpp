#include "super_ludo_player.h"

using namespace std;

void super_ludo_player::calc_fitness()
{
  // Own distance from start
  int own_total_dist_from_start = 0;
  for (size_t i = 0; i < 4; i++) {
    if (pos_start_of_turn[i] == 99)
      own_total_dist_from_start += 70; // Normaly only 57, but extra award!
    else if (pos_start_of_turn[i] == -1)
      own_total_dist_from_start += -10;
    else own_total_dist_from_start += pos_start_of_turn[i];
  }

  // Enemy distance from start
  int enemy_total_dist_from_start = 0;
  for (size_t i = 4; i < pos_start_of_turn.size(); i++) {
    if (pos_start_of_turn[i] == 99)
      enemy_total_dist_from_start += 70; // Normaly only 57, but extra award!
    else if (pos_start_of_turn[i] == -1)
      enemy_total_dist_from_start += -10;
    else{
      if (i < 8){
        if (pos_start_of_turn[i] < 13) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-13)+52;

        }else if (pos_start_of_turn[i] > 50) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-1*5);

        } else {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-13);

        }
      }
      else if (i < 12){
        if (pos_start_of_turn[i] < 26) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-26)+52;

        }else if (pos_start_of_turn[i] > 50) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-2*5);

        } else {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-26);

        }
      }
      else if (i < 16){
        if (pos_start_of_turn[i] < 39) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-39)+52;

        }else if (pos_start_of_turn[i] > 50) {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-3*5);

        } else {
          enemy_total_dist_from_start += (pos_start_of_turn[i]-39);

        }
      }
      else
        debug_stop("ERROR", 99, true);
    }
  }

  *my_fitness += (own_total_dist_from_start/4) - ((enemy_total_dist_from_start/3)/4);
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
  double fourth_largest = 90;
  double third_largest = 0;
  double second_largest = 0;
  double largest = 0;
  int fourth_largest_index = -99;
  int third_largest_index = -99;
  int second_largest_index = -99;
  int largest_index = -99;
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
  for (int i = 0; i < 4; ++i){
    if ( calc_out[i] < fourth_largest ){
      fourth_largest = calc_out[i];
      fourth_largest_index = i;
    }
  }

   return vector<int>{largest_index,second_largest_index,third_largest_index,fourth_largest_index};
}
