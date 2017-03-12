#include "super_ludo_player.h"
#include <random>
#include <game.h>

super_ludo_player::super_ludo_player(){
}

int super_ludo_player::make_decision(){

    for (int i = 0; i < 4; i++) {
      can_kill(pos_start_of_turn[i], dice_roll);
      can_get_home(pos_start_of_turn[i], dice_roll);
      can_enter_safe_zone(pos_start_of_turn[i], dice_roll);
      can_get_on_star(pos_start_of_turn[i], dice_roll);
      can_get_on_globe(pos_start_of_turn[i], dice_roll);
      can_enter_non_danger_zone(pos_start_of_turn[i], dice_roll);
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
bool super_ludo_player::can_kill(int pos, int new_dice_roll){
  if (pos == -1 || pos == 99)
    return false;

  int num_of_players = 0;

  for (int i = 4; i < pos_start_of_turn.size(); i++) {
    if ( pos_start_of_turn[i] == (pos+new_dice_roll) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 ){
      if (can_get_on_globe(pos_start_of_turn[i],0) || can_get_on_star(pos_start_of_turn[i],0)) {
        return false;
      }
      num_of_players++;
    } // TODO If star jump kill
  }

  if (num_of_players == 1) {
    // std::cout << "CAN KILL: " << dice_roll << std::endl;
    // std::cout << "POS: " << pos << std::endl;
    // for (size_t i = 0; i < 4; i++)
    // std::cout << "POS " << i << ": " << pos_start_of_turn[i] << std::endl;
    // std::cout << "Press ENTER to exit" << std::endl;
    // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_home(int pos, int new_dice_roll){
  if ( pos + new_dice_roll == 56 || pos + new_dice_roll == 50 ) {
    //std::cout << "CAN GET HOME: " << dice_roll << std::endl;
    //std::cout << "POS: " << pos << std::endl;
    //std::cout << "Press ENTER to exit" << std::endl;
    //std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_enter_safe_zone(int pos, int new_dice_roll){ // TODO Should 50 be included?
  if ( pos + new_dice_roll >= 50 && pos < 50) {
    // std::cout << "CAN ENTER SAFE: " << dice_roll << std::endl;
    // std::cout << "POS: " << pos << std::endl;
    // std::cout << "Press ENTER to exit" << std::endl;
    // std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_on_star(int pos, int new_dice_roll){ // TODO Problem with 39 red = 0: Is also here
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos+new_dice_roll))
    if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 || pos+new_dice_roll == 31 ||
       pos+new_dice_roll == 44 || pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
       pos+new_dice_roll == 37 || pos+new_dice_roll == 50 ) {
        //  Check for double players on new star?
        //  std::cout << "CAN GET ON STAR: " << dice_roll << std::endl;
        //  std::cout << "POS: " << pos << std::endl;
        //  std::cout << "Press ENTER to exit" << std::endl;
        //  std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
       return true;
    }
  return false;
}

bool super_ludo_player::can_get_on_globe(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos+new_dice_roll))
    if( pos+new_dice_roll % 13 == 0 || (pos+new_dice_roll - 8) % 13 == 0 ) { // TODO ADD TWO PALYERS ON TOP OF EACH OTHER (also, can we stack 3 on top?)
      //  std::cout << "CAN GET ON GLOBE: " << dice_roll << std::endl;
      //  std::cout << "POS: " << pos << std::endl;
      //  std::cout << "Press ENTER to exit" << std::endl;
      //  std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
       return true;
    }
  return false;
}

bool super_ludo_player::can_enter_non_danger_zone(int pos, int new_dice_roll){ // TODO TEST - NEED DICE_ROLL INCLUDED
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos+new_dice_roll))
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      for (int j = 1; j < 6; j++) {
        if (pos+new_dice_roll >= 6) {
          if (pos_start_of_turn[i] = (pos+new_dice_roll-j)) {
            return false;
          }
        }
        else if (false) {
          // TODO WRAP AROUND CODE
          return false;
        }
      }
    }
  return true;
}

bool super_ludo_player::currently_in_non_danger_zone(int pos){ // TODO TEST
  if (can_enter_non_danger_zone(pos,0)) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_in_safe_zone(int pos){
  if ( pos >= 51 ) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_on_globe(int pos){
  if ( can_get_on_globe(pos,0) ) {
    return true;
  }
  return false;
}

//TODO ADD TWO PLAYER ON SAME SPOT
bool super_ludo_player::enemy_globe(int pos){
  for (int i = 4; i < pos_start_of_turn.size(); i++) {
    if (can_get_on_globe(pos_start_of_turn[i],0)) {
      return true;
    }
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

// TODO:
// Will stacked players hit home people from own team?
// Two pieces of the same team counts as a globe and will send the piece that moves to it home.
// make func: can_survive_move
// fitness function
// neural network, to train after my play style
