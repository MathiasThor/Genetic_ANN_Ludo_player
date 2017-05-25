#include "neural_network.h"
using namespace std;

neural_network::neural_network( QApplication* app ){
  a = app;
}

void neural_network::learn(){
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  ludo_player_random p4_red;
  ludo_player_random p2_yellow;
  ludo_player_random p3_blue;
  super_ludo_player p1_green(true);

  game g;
  g.setGameDelay(10);

  Dialog w;
  QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
  QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
  QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
  QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
  QObject::connect(&g,SIGNAL(close()),a,SLOT(quit()));
  w.show();

  //set up for each player
  QObject::connect(&g,        SIGNAL(player1_start(positions_and_dice)),&p1_green,   SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                &g,           SLOT(movePiece(int)));
  QObject::connect(&g,        SIGNAL(player1_end(std::vector<int>)),    &p1_green,   SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              &g,           SLOT(turnComplete(bool)));

  QObject::connect(&g,         SIGNAL(player2_start(positions_and_dice)),&p2_yellow, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                &g,          SLOT(movePiece(int)));
  QObject::connect(&g,         SIGNAL(player2_end(std::vector<int>)),    &p2_yellow, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              &g,          SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player3_start(positions_and_dice)),&p3_blue, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                &g,        SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player3_end(std::vector<int>)),    &p3_blue, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              &g,        SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red, SIGNAL(select_piece(int)),                &g,      SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red, SIGNAL(turn_complete(bool)),              &g,      SLOT(turnComplete(bool)));

  for(int i = 0; i < 10000; ++i){
      g.start();
      a->exec();
      g.reset();
  }
}

void neural_network::play(){
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  ludo_player p4_red;
  ludo_player p2_yellow;
  ludo_player p3_blue;
  super_ludo_player p1_green(false);

  game g;
  g.setGameDelay(0);

  QObject::connect(&g,SIGNAL(close()),a,SLOT(quit()));

  //set up for each player
  QObject::connect(&g,        SIGNAL(player1_start(positions_and_dice)),&p1_green,   SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                &g,           SLOT(movePiece(int)));
  QObject::connect(&g,        SIGNAL(player1_end(std::vector<int>)),    &p1_green,   SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              &g,           SLOT(turnComplete(bool)));

  QObject::connect(&g,         SIGNAL(player2_start(positions_and_dice)),&p2_yellow, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                &g,          SLOT(movePiece(int)));
  QObject::connect(&g,         SIGNAL(player2_end(std::vector<int>)),    &p2_yellow, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              &g,          SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player3_start(positions_and_dice)),&p3_blue, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                &g,        SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player3_end(std::vector<int>)),    &p3_blue, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              &g,        SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red, SIGNAL(select_piece(int)),                &g,      SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red, SIGNAL(turn_complete(bool)),              &g,      SLOT(turnComplete(bool)));

  for(int i = 0; i < 10000; ++i){
      if (i%200 == 0)
        std::cout << "." << std::flush;
      g.start();
      a->exec();
      while (a->closingDown()){}
      g.reset();
      if(g.wait()){}
  }
  std::cout << "\n\nWins:\n" << (g.wins[3]+g.wins[1] + g.wins[2])/(double)3 << "\t" << g.wins[0] << endl;
}

void neural_network::star_gamer(){
  //QApplication a(argc, argv);
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  //
  // ludo_player p4_red;
  // ludo_player p2_yellow;
  // ludo_player p3_blue;

  ludo_player_random p4_red;
  ludo_player_random p2_yellow;
  ludo_player_random p3_blue;
  //

  // super_ludo_player p4_red(false);
  // super_ludo_player p2_yellow(false);
  // super_ludo_player p3_blue(false);


  super_ludo_player p1_green(false, "./../../GA-Player.net");

  game g;
  g.setGameDelay(0);

  QObject::connect(&g,SIGNAL(close()),a,SLOT(quit()));

  //set up for each player
  QObject::connect(&g,        SIGNAL(player1_start(positions_and_dice)),&p1_green,   SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                &g,           SLOT(movePiece(int)));
  QObject::connect(&g,        SIGNAL(player1_end(std::vector<int>)),    &p1_green,   SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              &g,           SLOT(turnComplete(bool)));

  QObject::connect(&g,         SIGNAL(player2_start(positions_and_dice)),&p2_yellow, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                &g,          SLOT(movePiece(int)));
  QObject::connect(&g,         SIGNAL(player2_end(std::vector<int>)),    &p2_yellow, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              &g,          SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player3_start(positions_and_dice)),&p3_blue, SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                &g,        SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player3_end(std::vector<int>)),    &p3_blue, SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              &g,        SLOT(turnComplete(bool)));

  QObject::connect(&g,       SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red, SIGNAL(select_piece(int)),                &g,      SLOT(movePiece(int)));
  QObject::connect(&g,       SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red, SIGNAL(turn_complete(bool)),              &g,      SLOT(turnComplete(bool)));

  for(int i = 0; i < 10000; ++i){
      if (i%200 == 0)
        std::cout << "." << std::flush;
      g.start();
      a->exec();
      while (a->closingDown()){}
      g.reset();
      if(g.wait()){}
  }
  std::cout << "\n\nWins:\n" << (g.wins[3]+g.wins[1] + g.wins[2])/(double)3 << "\t" << g.wins[0] << endl;
}
