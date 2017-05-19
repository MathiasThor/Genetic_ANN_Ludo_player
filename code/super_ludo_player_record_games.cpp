#include "super_ludo_player.h"

using namespace std;

// Function for making training data
int super_ludo_player::record_my_games(){
  cout << "\n---\n\n";

  cout << "Dice Roll: "<< dice_roll << endl;
  for (int i = 0; i < 4; i++) {
    std::cout << "Player " << i << " Position: " << pos_start_of_turn[i] << std::endl;
  }

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
    if ( !can_move(pos_start_of_turn[player_num], dice_roll) ) {
      cout << "can't move this player\nenter new player to move: ";
    } else break;
  }

  ofstream myfile;
  myfile.open ("./../../data/plays_data/plays.data", ios::app);

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
    myfile <<  can_move(pos_start_of_turn[i], dice_roll)<< " ";
    myfile <<  currently_on_enemy_start(pos_start_of_turn[i])<< " ";
    myfile <<  currently_in_safe_zone(pos_start_of_turn[i])<< " ";
    myfile <<  currently_in_non_danger_zone(pos_start_of_turn[i])<< " ";
    myfile <<  currently_on_globe(pos_start_of_turn[i])<< " ";
    myfile <<  currently_on_star(pos_start_of_turn[i])<< " ";
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
