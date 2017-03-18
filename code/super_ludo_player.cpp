#include "super_ludo_player.h"
#include <random>
#include <game.h>
#include <iostream>
#include <fstream>
#include "floatfann.h"
using namespace std;

super_ludo_player::super_ludo_player():
  pos_start_of_turn(16),
  pos_end_of_turn(16),
  dice_roll(0)
{
}

int super_ludo_player::make_decision(){
  fann_type *calc_out;
  fann_type input[56];
  struct fann *ann = fann_create_from_file("./../../ludo_player.net");
  return -1;
}

// OK
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
    debug_stop("KILL PLAYER", pos, true);
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::can_get_home(int pos, int new_dice_roll){ // Seems OK (few random plays)
  if ( pos + new_dice_roll == 56 || pos + new_dice_roll == 50 ) {
    //debug_stop("GET HOME", pos, false);
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::can_enter_safe_zone(int pos, int new_dice_roll){ // Seems OK (few random plays)
  if ( pos + new_dice_roll >= 51 && pos < 51) {
    //debug_stop("ENTER SAFE", pos, false);
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::can_get_on_star(int pos, int new_dice_roll){ // OK
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos))
    if(pos+new_dice_roll == 5  || pos+new_dice_roll == 18 || pos+new_dice_roll == 31 ||
       pos+new_dice_roll == 44 || pos+new_dice_roll == 11 || pos+new_dice_roll == 24 ||
       pos+new_dice_roll == 37 || pos+new_dice_roll == 50 ) {
       //debug_stop("GET ON STAR", pos, false);
       return true;
    }
  return false;
}

// OK
bool super_ludo_player::can_get_on_globe(int pos, int new_dice_roll){
  int stack_player_count = 0;

  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos) && pos+new_dice_roll < 51){
    if( (pos+new_dice_roll - 8) % 13 == 0 ) {
      //debug_stop("GET ON GLOBE", pos, true);
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
        //debug_stop("2 man stack(1)", pos, true);
        return true;
      } else if (new_dice_roll == 0 && stack_player_count >= 2) {
        //debug_stop("2 man stack(2)", pos, false);
        return true;
      }
    }
  }
  return false;
}

// OK
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

// OK
bool super_ludo_player::currently_home(int pos){
  if ( pos == 99 ) {
    //debug_stop("HOME", pos, false);
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::currently_in_non_danger_zone(int pos){
  if (can_enter_non_danger_zone(pos,0)) {
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::currently_in_safe_zone(int pos){
  if ( pos >= 51 && pos != 99 ) {
    //debug_stop("Is in safe", pos, true);
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::currently_on_globe(int pos){
  if ( can_get_on_globe(pos,0) || pos == 0) {
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::can_get_on_enemy_start(int pos, int new_dice_roll){
  if ( pos != -1 && pos != 99 && !currently_in_safe_zone(pos) && pos+new_dice_roll < 51){
    if ((pos+new_dice_roll) % 13 == 0) {
      return true;
    }
  }
  return false;
}

// OK
bool super_ludo_player::currently_on_enemy_start(int pos){
  if ( can_get_on_enemy_start(pos,0) && pos != 0) {
    return true;
  }
  return false;
}

// OK
bool super_ludo_player::can_get_killed(int pos, int new_dice_roll){
  if (pos == -1 || pos == 99 || currently_in_safe_zone(pos) || pos+new_dice_roll > 50)
    return false;

  if (enemy_globe(pos+new_dice_roll)) {
    return true;
  }
  return false;
}

//OK
bool super_ludo_player::can_get_out_of_start(int pos, int new_dice_roll){
  if (pos == -1 && new_dice_roll == 6) {
    return true;
  }
  return false;
}

// OK
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

void super_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = record_my_games();//make_decision();
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

int super_ludo_player::record_my_games(){
  cout << "\n---\n\n";

  for (int i = 0; i < 4; i++) {
    std::cout << "Player " << i << " Position: " << pos_start_of_turn[i] << std::endl;
  }

  // TODO THIS FUNCTION SHOULD ALSO BE IN THE FINAL ONE
  if (dice_roll != 6 &&
    (pos_start_of_turn[0] == -1 || pos_start_of_turn[0] == 99) &&
    (pos_start_of_turn[1] == -1 || pos_start_of_turn[1] == 99) &&
    (pos_start_of_turn[2] == -1 || pos_start_of_turn[2] == 99) &&
    (pos_start_of_turn[3] == -1 || pos_start_of_turn[3] == 99)) {
    return -1;
  }

  cout << "enter player to move: ";
  int player_num = 0;
  while (true) {
    cin >> player_num;
    cout << endl;
    if ((pos_start_of_turn[player_num] == -1 && dice_roll != 6) || pos_start_of_turn[player_num] == 99) {
      cout << "can't move this player\nenter new player to move: ";
    } else break;
  }

  ofstream myfile;
  myfile.open ("plays.data", ios::app);

  for (int i = 0; i < 4; i++) {
    myfile <<  can_kill(pos_start_of_turn[i], dice_roll) << " ";
    myfile <<  can_get_home(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_enter_safe_zone(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_get_on_star(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_get_on_globe(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_enter_non_danger_zone(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_get_killed(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_get_out_of_start(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  can_get_on_enemy_start(pos_start_of_turn[i], dice_roll)<< " ";

    myfile <<  currently_on_enemy_start(pos_start_of_turn[i])<< " ";
    myfile <<  currently_in_safe_zone(pos_start_of_turn[i])<< " ";
    myfile <<  currently_in_non_danger_zone(pos_start_of_turn[i])<< " ";
    myfile <<  currently_on_globe(pos_start_of_turn[i])<< " ";
    myfile <<  currently_home(pos_start_of_turn[i])<< " ";
  }

  myfile <<  "\n";

  switch (player_num) {
    case 0:
      myfile << "1 0 0 0";
      break;
    case 1:
      myfile << "0 1 0 0";
      break;
    case 2:
      myfile << "0 0 1 0";
      break;
    case 3:
      myfile << "0 0 0 1";
      break;
    default:
      myfile << "ERROR";
      break;
  }

  myfile <<  "\n";
  myfile.close();
  return player_num;
}


// TODO's:
// Will stacked players hit home people from own team? NO - SEE "send_them_home"

// fitness function: f = WINNER*? + players_home*? + leftover_distance*?
// Neural network, to train after my play style
