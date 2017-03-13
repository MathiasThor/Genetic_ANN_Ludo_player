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
      currently_in_safe_zone(pos_start_of_turn[i]);
      currently_in_non_danger_zone(pos_start_of_turn[i]);
      currently_on_globe(pos_start_of_turn[i]);
      enemy_globe(pos_start_of_turn[i]);
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

bool super_ludo_player::can_kill(int pos, int new_dice_roll){ // OK - HAVENT SEEN STAR KILL YET
  if (pos == -1 || pos == 99 || currently_in_safe_zone(pos))
    return false;

  int num_of_players = 0;

  if (can_get_on_star(pos,new_dice_roll)) {
    // STAR JUMP KILL
    int star_jump;
    if(pos == 5  || pos == 18 ||
       pos == 31 || pos == 44){
        star_jump = 6;
    }
    else if(pos == 11 || pos == 24 ||
            pos == 37 || pos == 50){
        star_jump = 7;
    }
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      if ( pos_start_of_turn[i] == (pos+new_dice_roll+star_jump) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 ){
        if (can_get_on_globe(pos_start_of_turn[i],0) || can_get_on_star(pos_start_of_turn[i],0)) {
          return false;
        }
        num_of_players++;
      }
    }
  }
  else {
    // NORMAL KILL
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      if ( pos_start_of_turn[i] == (pos+new_dice_roll) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 ){
        if (can_get_on_globe(pos_start_of_turn[i],0) || can_get_on_star(pos_start_of_turn[i],0)) {
          return false;
        }
        num_of_players++;
      }
    }
  }

  if (num_of_players == 1) {
    debug_stop("KILL PLAYER", pos, true);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_home(int pos, int new_dice_roll){
  if ( pos + new_dice_roll == 56 || pos + new_dice_roll == 50 ) {
    //debug_stop("GET HOME", pos, false);
    return true;
  }
  return false;
}

bool super_ludo_player::can_enter_safe_zone(int pos, int new_dice_roll){
  if ( pos + new_dice_roll >= 51 && pos < 51) {
    //debug_stop("ENTER SAFE", pos, false);
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_on_star(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !can_enter_safe_zone(pos,new_dice_roll))
    if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 || pos+new_dice_roll == 31 ||
       pos+new_dice_roll == 44 || pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
       pos+new_dice_roll == 37 || pos+new_dice_roll == 50 ) {
       //debug_stop("GET ON STAR", pos, false);
       return true;
    }
  return false;
}

bool super_ludo_player::can_get_on_globe(int pos, int new_dice_roll){
  int stack_player_count = 0;

  if ( pos != -1 && pos != 99 && !can_enter_safe_zone(pos,new_dice_roll)){
    if( pos+new_dice_roll % 13 == 0 || (pos+new_dice_roll - 8) % 13 == 0 ) {
      //debug_stop("GET ON GLOBE", pos, true);
      return true;
    } else {
      if (can_get_on_star(pos,new_dice_roll)) {
        // STAR JUMP KILL
        int star_jump;
        if(pos == 5  || pos == 18 ||
           pos == 31 || pos == 44){
            star_jump = 6;
        }
        else if(pos == 11 || pos == 24 ||
                pos == 37 || pos == 50){
            star_jump = 7;
        }
        for (int i = 0; i < 4; i++) {
          if (pos + new_dice_roll + star_jump == pos_start_of_turn[i])
            stack_player_count++;
        }
      } else {
        for (int i = 0; i < 4; i++) {
          if (pos + new_dice_roll == pos_start_of_turn[i])
            stack_player_count++;
        }
      }
      if (new_dice_roll != 0 && stack_player_count >= 1) {
        //debug_stop("2X PLAYER(1)", pos, true);
        return true;
      } else if (new_dice_roll == 0 && stack_player_count >= 2) {
        //debug_stop("2X PLAYER(2)", pos, false);
        return true;
      }
    }
  }
  return false;
}

bool super_ludo_player::can_enter_non_danger_zone(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !can_enter_safe_zone(pos,new_dice_roll))
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      for (int j = 1; j < 6; j++) {
        if (pos+new_dice_roll >= 6) {
          if (pos_start_of_turn[i] == (pos+new_dice_roll-j)) {
            return false;
          }
        }
        else{
          if (pos_start_of_turn[i] == 52+(pos+new_dice_roll-j)) {
            return false;
          }
        }
      }
    }
  return true;
}

bool super_ludo_player::currently_in_non_danger_zone(int pos){
  if (can_enter_non_danger_zone(pos,0)) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_in_safe_zone(int pos){
  if ( pos >= 51 && pos != 99 ) {
    //debug_stop("Is in safe", pos, true);
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

// TODO's:
// Will stacked players hit home people from own team?
// Can we can we stack 3 players
// Two pieces of the same team counts as a globe and will send the piece that moves to it home.
// make func: can_survive_move
// fitness function: f = WINNER*? + players_home*? + leftover_distance*?
// neural network, to train after my play style
