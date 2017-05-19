#include "super_ludo_player.h"

using namespace std;

//
//
//  THIS FILES CONTAINS ALL THE STATE FUNCTION!
//
//

bool super_ludo_player::can_kill(int pos, int new_dice_roll){ // OK + STAR KILL OK
  if (pos == -1 || pos == 99 || currently_in_safe_zone(pos) && pos+new_dice_roll > 50)
    return false;

  int num_of_players = 0;
  int star_jump = 0;

  if (can_get_on_star(pos,new_dice_roll)) {
    // STAR JUMP KILL
    if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 ||
       pos+new_dice_roll == 31 || pos+new_dice_roll == 44){
        star_jump = 6;
    }
    else if(pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
            pos+new_dice_roll == 37 || pos+new_dice_roll == 50){
        star_jump = 7;
    }
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      if ( pos_start_of_turn[i] == (pos+new_dice_roll+star_jump) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 && !currently_in_safe_zone(pos_start_of_turn[i]) && !currently_on_globe(pos_start_of_turn[i])){
        num_of_players++;
      }
    }
  }
  else {
    // NORMAL KILL
    for (int i = 4; i < pos_start_of_turn.size(); i++) {
      if ( pos_start_of_turn[i] == (pos+new_dice_roll) && pos_start_of_turn[i] != 99 && pos_start_of_turn[i] != -1 ){
        if (currently_on_globe(pos_start_of_turn[i]) || can_get_on_star(pos_start_of_turn[i],0) || currently_on_enemy_start(pos_start_of_turn[i])) {
          return false;
        }
        num_of_players++;
      }
    }
  }

  if (num_of_players == 1 && star_jump != 0) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_home(int pos, int new_dice_roll){
  if ( pos + new_dice_roll == 56 || pos + new_dice_roll == 50 ) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_enter_safe_zone(int pos, int new_dice_roll){
  if ( pos + new_dice_roll >= 51 && pos < 51) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_move(int pos, int new_dice_roll){
  if ((pos == -1 && new_dice_roll != 6) || pos == 99)
    return false;
  return true;
}

bool super_ludo_player::can_get_on_star(int pos, int new_dice_roll){ // OK
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos))
    if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 || pos+new_dice_roll == 31 ||
       pos+new_dice_roll == 44 || pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
       pos+new_dice_roll == 37 || pos+new_dice_roll == 50 ) {
       return true;
    }
  return false;
}

bool super_ludo_player::can_get_on_globe(int pos, int new_dice_roll){
  int stack_player_count = 0;

  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos) && pos+new_dice_roll < 51){
    if( (pos+new_dice_roll - 8) % 13 == 0 ) {
      return true;
    } else {
      if (can_get_on_star(pos,new_dice_roll)) {
        int star_jump;
        if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 ||
           pos+new_dice_roll == 31 || pos+new_dice_roll == 44){
            star_jump = 6;
        }
        else if(pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
                pos+new_dice_roll == 37 || pos+new_dice_roll == 50){
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
        return true;
      } else if (new_dice_roll == 0 && stack_player_count >= 2) {
        return true;
      }
    }
  }
  return false;
}

bool super_ludo_player::can_enter_non_danger_zone(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos)){
    if (can_get_on_star(pos,new_dice_roll)) {
      int star_jump;
      if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 ||
         pos+new_dice_roll == 31 || pos+new_dice_roll == 44){
          star_jump = 6;
      }
      else if(pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
              pos+new_dice_roll == 37 || pos+new_dice_roll == 50){
          star_jump = 7;
      }
      for (int i = 4; i < pos_start_of_turn.size(); i++) {
        for (int j = 1; j < 6; j++) {
          if (pos+new_dice_roll+star_jump >= 6) {
            if (pos_start_of_turn[i] == (pos+new_dice_roll+star_jump-j)) {
              return false;
            }
          }
          else{
            if (pos_start_of_turn[i] == 52+(pos+new_dice_roll+star_jump-j)) {
              return false;
            }
          }
        }
      }
    } else {
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
    }
    return true;
  }
  return false;
}

bool super_ludo_player::currently_home(int pos){
  if ( pos == 99 ) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_in_non_danger_zone(int pos){
  if (can_enter_non_danger_zone(pos,0)) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_in_safe_zone(int pos){
  if ( pos >= 51 && pos != 99 ) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_on_globe(int pos){
  if ( can_get_on_globe(pos,0) || pos == 0) {
    return true;
  }
  return false;
}

bool super_ludo_player::currently_on_star(int pos){
  if ( can_get_on_star(pos,0) || pos == 0) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_on_enemy_start(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos) && pos+new_dice_roll < 51){
    if ((pos+new_dice_roll) % 13 == 0) {
      return true;
    }
  }
  return false;
}

bool super_ludo_player::currently_on_enemy_start(int pos){
  if ( can_get_on_enemy_start(pos,0) && pos != 0) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_killed(int pos, int new_dice_roll){
  if (pos == -1 || pos == 99 || currently_in_safe_zone(pos) || pos+new_dice_roll > 50)
    return false;

  if (enemy_globe(pos+new_dice_roll)) {
    return true;
  }
  return false;
}

bool super_ludo_player::can_get_out_of_start(int pos, int new_dice_roll){
  if (pos == -1 && new_dice_roll == 6) {
    return true;
  }
  return false;
}

bool super_ludo_player::enemy_globe(int pos){
  if (pos == -1 || pos == 99 || currently_in_safe_zone(pos))
    return false;

  for (int i = 4; i < pos_start_of_turn.size(); i++) {
    if ((pos == pos_start_of_turn[i] && (currently_on_globe(pos_start_of_turn[i])) || currently_on_enemy_start(pos_start_of_turn[i])) ) {
      return true;
    }
  }
  return false;
}
