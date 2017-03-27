#include "super_ludo_player.h"

using namespace std;

super_ludo_player::super_ludo_player():
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0)
{
}

int super_ludo_player::make_decision(){

  if (dice_roll != 6 &&
    (pos_start_of_turn[0] == -1 || pos_start_of_turn[0] == 99) &&
    (pos_start_of_turn[1] == -1 || pos_start_of_turn[1] == 99) &&
    (pos_start_of_turn[2] == -1 || pos_start_of_turn[2] == 99) &&
    (pos_start_of_turn[3] == -1 || pos_start_of_turn[3] == 99)) {
    return -1;
  }

  fann_type *calc_out;
  fann_type input[60];
  FANN::neural_net net;
  net.create_from_file("./../../ann_code/ludo_player.net");

  // TODO: REMOVE
  struct fann_connection 	*connections;
  unsigned int num_connections;

  num_connections = net.get_total_connections();
  connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  chromosome new_chromo = get_chromosome(connections, num_connections);

  chromosome new_chromo_with_gaus = add_gaussian_noise_to_chromosome(new_chromo);

  std::cout << bitset_to_double(new_chromo[1]) << "\nvs.\n" << bitset_to_double(new_chromo_with_gaus[1]) << endl;


  debug_stop("check", 99, false);
  // TODO: REMOVE

  for (int i = 0; i < 4; i++) {
    input[0+(i*15)] =can_kill(pos_start_of_turn[i], dice_roll);
    input[1+(i*15)] =can_get_home(pos_start_of_turn[i], dice_roll);
    input[2+(i*15)] =can_enter_safe_zone(pos_start_of_turn[i], dice_roll);
    input[3+(i*15)] =can_get_on_star(pos_start_of_turn[i], dice_roll);
    input[4+(i*15)] =can_get_on_globe(pos_start_of_turn[i], dice_roll);
    input[5+(i*15)] =can_enter_non_danger_zone(pos_start_of_turn[i], dice_roll);
    input[6+(i*15)] =can_get_killed(pos_start_of_turn[i], dice_roll);
    input[7+(i*15)] =can_get_out_of_start(pos_start_of_turn[i], dice_roll);
    input[8+(i*15)] =can_get_on_enemy_start(pos_start_of_turn[i], dice_roll);
    input[9+(i*15)] =can_move(pos_start_of_turn[i], dice_roll);
    input[10+(i*15)] =currently_on_enemy_start(pos_start_of_turn[i]);
    input[11+(i*15)] =currently_in_safe_zone(pos_start_of_turn[i]);
    input[12+(i*15)] =currently_in_non_danger_zone(pos_start_of_turn[i]);
    input[13+(i*15)] =currently_on_globe(pos_start_of_turn[i]);
    input[14+(i*15)] =currently_home(pos_start_of_turn[i]);
  }

  calc_out = net.run(input);

  vector<int> sorted_indexx = sorted_index(calc_out);

  net.destroy();

  if ( !can_move(pos_start_of_turn[sorted_indexx[0]], dice_roll) ){
    if ( !can_move(pos_start_of_turn[sorted_indexx[1]], dice_roll) ) {
      if ( !can_move(pos_start_of_turn[sorted_indexx[2]], dice_roll) ) {
        if ( !can_move(pos_start_of_turn[sorted_indexx[3]], dice_roll) ) {
          std::cout << "*** ERROR ***" << std::endl;
          std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
        } else {
          cout << "- ";
          // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
          return sorted_indexx[3];
        }
      } else {
        cout << "+ ";
        // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
        return sorted_indexx[2];
      }
    } else {
      cout << "* ";
      // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
      return sorted_indexx[1];
    }
  } else {
    return sorted_indexx[0];
  }

}

void super_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    //int decision = record_my_games();
    emit select_piece(decision);
}

void super_ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}


// Will stacked players hit home people from own team? NO - SEE "send_them_home"
// fitness function: f = WINNER*? + players_home*? + leftover_distance*?
// Neural network, to train after my play style
