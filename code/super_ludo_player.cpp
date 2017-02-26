#include "super_ludo_player.h"
#include <random>
#include <game.h>

super_ludo_player::super_ludo_player(){
}

int super_ludo_player::make_decision(){

    for (int i = 0; i < 4; i++) {
      can_kill(pos_start_of_turn[i]);
      can_get_home(pos_start_of_turn[i]);
      can_enter_safe_zone(pos_start_of_turn[i]);
      can_get_on_star(pos_start_of_turn[i]);
    }

    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

// TODO Investigate bug --> Something when they are in start position the relative pos does not work?
// (blue 26 and red 39) :(
bool super_ludo_player::can_kill(int pos){
  if (pos == -1 || pos == 99)
    return false;

  int num_of_players = 0;

  for (int i = 4; i < pos_start_of_turn.size(); i++) { //TODO: add if on globe or star
    if ( pos_start_of_turn[i] == (pos+dice_roll) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 ) // TODO USE IF STAR AND IS GLOBE
      num_of_players++;
  }

  if (num_of_players == 1) {
    // std::cout << "CAN KILL: " << dice_roll << std::endl;
    // std::cout << "POS: " << pos << std::endl;
    // for (size_t i = 0; i < 4; i++)
    //   std::cout << "POS " << i << ": " << pos_start_of_turn[i] << std::endl;
    // std::cout << "Press ENTER to exit" << std::endl;
    // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_home(int pos){
  if ( pos + dice_roll == 56 || pos + dice_roll == 50 ) {
    //std::cout << "CAN GET HOME: " << dice_roll << std::endl;
    //std::cout << "POS: " << pos << std::endl;
    //std::cout << "Press ENTER to exit" << std::endl;
    //std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_enter_safe_zone(int pos){ // TODO Should 50 be included?
  if ( pos + dice_roll >= 50 && pos < 50) {
    // std::cout << "CAN ENTER SAFE: " << dice_roll << std::endl;
    // std::cout << "POS: " << pos << std::endl;
    // std::cout << "Press ENTER to exit" << std::endl;
    // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_on_star(int pos){ // TODO Problem with 39 red = 0: Is also here
  if ( pos != -1 && pos != 99 )
    if(pos+dice_roll == 5  || pos+dice_roll == 18 || pos+dice_roll == 31 ||
       pos+dice_roll == 44 || pos+dice_roll == 11 || pos+dice_roll == 24 ||
       pos+dice_roll == 37 || pos+dice_roll == 50 ) {
        //  std::cout << "CAN GET ON STAR: " << dice_roll << std::endl;
        //  std::cout << "POS: " << pos << std::endl;
        //  std::cout << "Press ENTER to exit" << std::endl;
        //  std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
         return true;
    }
  return false;
}

bool super_ludo_player::can_get_on_globe(int pos){
  if ( pos != -1 && pos != 99 )
    if( index % 13 == 0 || (index - 8) % 13 == 0 ) { // TODO ADD TWO MATES ON TOP OF EACH OTHER (also, can we stack 3 on top?)
         return true;
    }
  return false;
}

bool super_ludo_player::can_enter_danger_zone(int pos){
  //TODO 6 or less steps after an opponent player
  return false;
}

bool super_ludo_player::is_brick_in_safe_zone(int pos){
  if ( pos >= 51 ) {
    return true;
  }
  return false;
}

void super_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
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
