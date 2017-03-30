#include "genetic_algorithm.h"
using namespace std;

genetic_algorithm::genetic_algorithm(int new_argc, char *new_argv[]){
  argc = new_argc;
  argv = new_argv;

  net.create_from_file("./../../ann_code/ludo_player.net");

  unsigned int num_connections = net.get_total_connections();
  struct fann_connection *connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  super_population.push_back( get_chromosome( connections, num_connections) );

  for (size_t i = 1; i < POP_SIZE; i++)
    super_population.push_back( add_gaussian_noise_to_chromosome( super_population[0]) );

  cout << "super_population 1: " << connections[0].weight << " = "<< super_population[0][0] << " = "<< bitset_to_float(super_population[0][0]) << endl;
  debug_stop("");
  //Set gaussian noised chromosome as current
  //set_chromosome_as_weights(super_population[0]);

  play_game(super_population[0]);
}

bitset<32> genetic_algorithm::float_to_bitset(float input_float){
  data.input = input_float;
  bitset<32>   bits(data.output);
  return bits;
}

float genetic_algorithm::bitset_to_float(bitset<32> input_set){
  uint32_t const u = input_set.to_ullong();
  float d;

  // Aliases to `char*` are explicitly allowed in the Standard (and only them)
  char const* cu = reinterpret_cast<char const*>(&u);
  char* cd = reinterpret_cast<char*>(&d);

  // Copy the bitwise representation from u to input_set
  memcpy(cd, cu, sizeof(u));
  return d;
}

chromosome genetic_algorithm::get_chromosome( fann_connection* connections , unsigned int num_connections){
  chromosome new_chromo;

  for (int i = 0; i < num_connections; ++i)
    new_chromo.push_back( float_to_bitset(connections[i].weight) );

  return new_chromo;
}

chromosome genetic_algorithm::add_gaussian_noise_to_chromosome( chromosome input_chromo ){
  chromosome output_chromo;

  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = 1.0;
  double tmp = 0;
  std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
  std::normal_distribution<double> dist(mean, stddev);

  // Add Gaussian noise
  for (size_t i = 0; i < input_chromo.size(); i++) {
    tmp = bitset_to_float(input_chromo[i]);
    tmp += dist(generator);
    output_chromo.push_back( float_to_bitset(tmp) );
  }

  return output_chromo;
}

void genetic_algorithm::debug_stop(std::string action){
 std::cout << action << std::endl;
 std::cout << "Press ENTER to continue" << std::endl;
 std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
}

void genetic_algorithm::play_game(chromosome player_chromo){
  QApplication a(argc, argv);
  qRegisterMetaType<positions_and_dice>();

  //instanciate the players here
  ludo_player_random p1_green;
  ludo_player_random p2_yellow;
  ludo_player_random p3_blue;
  super_ludo_player p4_red(player_chromo);

  game g;
  g.setGameDelay(0); //if you want to see the game, set a delay

  /* Add a GUI <-- remove the '/' to uncomment block
  Dialog w;
  QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
  QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
  QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
  QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
  QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
  w.show();
  /*/ //Or don't add the GUI
  QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
  //*/

  //set up for each player
  QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1_green,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p1_green,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
  QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1_green,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

  QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2_yellow,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
  QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2_yellow,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

  QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3_blue,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
  QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3_blue,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

  QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
  QObject::connect(&p4_red,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
  QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
  QObject::connect(&p4_red,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

  for(int i = 0; i < 1000; ++i){
      g.start();
      a.exec();
      g.reset();
  }

  std::cout << "Player 0 (Green)  Won " << g.wins[0] << " games" << std::endl;
  std::cout << "Player 1 (Yellow) Won " << g.wins[1] << " games" << std::endl;
  std::cout << "Player 2 (Blue)   Won " << g.wins[2] << " games" << std::endl;
  std::cout << "Player 3 (Red)    Won " << g.wins[3] << " games" << std::endl;

  // p1_green.~ludo_player_random();
  // p2_yellow.~ludo_player_random();
  // p3_blue.~ludo_player_random();
  // delete p4_red;
}

// TODO: THE FOLLOWING I ABLE TO RESIZE
// for (size_t i = 0; i < 8; i++)
//   super_population[0][0].reset(i);
//
// std::bitset<24> hello;
//
// for (size_t i = 31; i > 7; i--)
//   hello[i-8] = super_population[0][0][i];
//
// cout << endl;
// cout << hello << "\n" << super_population[0][0] << endl;
