#include "genetic_algorithm.h"
using namespace std;

int genetic_algorithm::play_game(chromosome player_chromo, float *fitness){
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  ludo_player_random p4_red;
  ludo_player_random p2_yellow;
  ludo_player_random p3_blue;
  super_ludo_player p1_green(player_chromo, fitness);

  game *g = new game;
  g->setGameDelay(0); //if you want to see the game, set a delay

  /* Add a GUI <-- remove the '/' to uncomment block
  Dialog w;
  QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
  QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
  QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
  QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
  QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
  w.show();
  /*/ //Or don't add the GUI
  QObject::connect(g,SIGNAL(close()),a,SLOT(quit()));
  //*/

  //set up for each player
  QObject::connect(g,        SIGNAL(player1_start(positions_and_dice)),&p1_green,   SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                g,           SLOT(movePiece(int)));
  QObject::connect(g,        SIGNAL(player1_end(std::vector<int>)),    &p1_green,   SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              g,           SLOT(turnComplete(bool)));

  QObject::connect(g,         SIGNAL(player2_start(positions_and_dice)),&p2_yellow, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                g,          SLOT(movePiece(int)));
  QObject::connect(g,         SIGNAL(player2_end(std::vector<int>)),    &p2_yellow, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              g,          SLOT(turnComplete(bool)));

  QObject::connect(g,       SIGNAL(player3_start(positions_and_dice)),&p3_blue, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                g,        SLOT(movePiece(int)));
  QObject::connect(g,       SIGNAL(player3_end(std::vector<int>)),    &p3_blue, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              g,        SLOT(turnComplete(bool)));

  QObject::connect(g,       SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red, SIGNAL(select_piece(int)),                g,      SLOT(movePiece(int)));
  QObject::connect(g,       SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red, SIGNAL(turn_complete(bool)),              g,      SLOT(turnComplete(bool)));

  g->start();
  a->exec();

  // for(int i = 0; i < 10; ++i){
  //     //std::cout << 1 << std::flush;
  //     g.start();
  //     //std::cout << 2 << std::flush;
  //     a.exec();
  //     //std::cout << 3 << std::flush;
  //     g.reset();
  //     //std::cout << 4 << std::flush;
  // }

  //std::vector<int> v { g.wins[0], g.wins[1], g.wins[2], g.wins[3] };
  return g->wins[3];
}

int genetic_algorithm::play_turnament(chromosome player1_chromo, chromosome player2_chromo, chromosome player3_chromo, chromosome player4_chromo){
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  super_ludo_player p1_green(player1_chromo);
  super_ludo_player p2_yellow(player2_chromo);
  super_ludo_player p3_blue(player3_chromo);
  super_ludo_player p4_red(player4_chromo);

  game *g = new game;
  g->setGameDelay(0); //if you want to see the game, set a delay

  QObject::connect(g,SIGNAL(close()),a,SLOT(quit()));

  //set up for each player
  QObject::connect(g, SIGNAL(player1_start(positions_and_dice)),&p1_green,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                g, SLOT(movePiece(int)));
  QObject::connect(g, SIGNAL(player1_end(std::vector<int>)),    &p1_green,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              g, SLOT(turnComplete(bool)));

  QObject::connect(g, SIGNAL(player2_start(positions_and_dice)),&p2_yellow,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                g, SLOT(movePiece(int)));
  QObject::connect(g, SIGNAL(player2_end(std::vector<int>)),    &p2_yellow,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              g, SLOT(turnComplete(bool)));

  QObject::connect(g, SIGNAL(player3_start(positions_and_dice)),&p3_blue,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                g, SLOT(movePiece(int)));
  QObject::connect(g, SIGNAL(player3_end(std::vector<int>)),    &p3_blue,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              g, SLOT(turnComplete(bool)));

  QObject::connect(g, SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red,SIGNAL(select_piece(int)),                g, SLOT(movePiece(int)));
  QObject::connect(g, SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red,SIGNAL(turn_complete(bool)),              g, SLOT(turnComplete(bool)));

  g->start();
  a->exec();

  // for(int i = 0; i < 10; ++i){
  //     g.start();
  //     a.exec();
  //     g.reset();
  // }

  std::vector<int> v { g->wins[0], g->wins[1], g->wins[2], g->wins[3] };

  int largest = 0;
  int largest_index = 0;
  for(int i = 0; i < 4; i++)
  {
      if( g->wins[i] > largest ){
        largest=g->wins[i];
        largest_index = i;
      }
  }
  return largest_index;
}
