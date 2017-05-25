#include "super_ludo_player.h"

using namespace std;

super_ludo_player::super_ludo_player(chromosome player_chromo, float *fitness):
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0)
{
  my_fitness = fitness;
  learn = false;
  calculate_fitness = true;
  net.create_from_file("./../../ANN-player.net");
  set_chromosome_as_weights(player_chromo);
}

super_ludo_player::super_ludo_player(chromosome player_chromo):
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0)
{
  my_fitness = NULL;
  learn = false;
  calculate_fitness = false;
  net.create_from_file("./../../ANN-player.net");
  set_chromosome_as_weights(player_chromo);
}

super_ludo_player::super_ludo_player(bool do_learning):
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0),
  learn(do_learning)
{
  my_fitness = NULL;
  calculate_fitness = false;
  if (!do_learning)
    net.create_from_file("./../../ANN-player.net");
}

super_ludo_player::super_ludo_player(bool do_learning, std::string load_from):
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0),
  learn(do_learning)
{
  my_fitness = NULL;
  calculate_fitness = false;
  if (!do_learning)
    net.create_from_file(load_from);
}

super_ludo_player::~super_ludo_player()
{
  net.destroy();
}

int super_ludo_player::make_decision(){

  if (dice_roll != 6 &&
    (pos_start_of_turn[0] == -1 || pos_start_of_turn[0] == 99) &&
    (pos_start_of_turn[1] == -1 || pos_start_of_turn[1] == 99) &&
    (pos_start_of_turn[2] == -1 || pos_start_of_turn[2] == 99) &&
    (pos_start_of_turn[3] == -1 || pos_start_of_turn[3] == 99)) {
    return 1;
  }

  fann_type *calc_out;
  fann_type input[60];

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
    input[14+(i*15)] =currently_on_star(pos_start_of_turn[i]);
  }

  calc_out = net.run(input);

  vector<int> sorted_indexx = sorted_index(calc_out);

  if ( !can_move(pos_start_of_turn[sorted_indexx[0]], dice_roll) ){
    if ( !can_move(pos_start_of_turn[sorted_indexx[1]], dice_roll) ) {
      if ( !can_move(pos_start_of_turn[sorted_indexx[2]], dice_roll) ) {
        if ( !can_move(pos_start_of_turn[sorted_indexx[3]], dice_roll) ) {
        } else {
          return sorted_indexx[3];
        }
      } else {
        return sorted_indexx[2];
      }
    } else {
      return sorted_indexx[1];
    }
  } else {
    return sorted_indexx[0];
  }
  return 1;
}

void super_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = 99;

    if (learn)
      decision = record_my_games();
    else
      decision = make_decision();

    if(calculate_fitness)
      calc_fitness();

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
