#include "genetic_algorithm.h"
using namespace std;

genetic_algorithm::genetic_algorithm(int new_argc, char *new_argv[]){
  argc = new_argc;
  argv = new_argv;

  ////////////////////////////////
  // INIT POPULATION W. GAUSSIAN
  ////////////////////////////////
  init_population();

  ////////////////////////////////
  // EVALUATION
  ////////////////////////////////
  evaluation();
  float fitness = 0;
  std::vector<int> wins = play_game(super_population[0], fitness);

  cout << "\nfitness out: "<< fitness << endl << endl;

  std::cout << "Player 0 (Green)  Won " << wins[0] << " games" << std::endl;
  std::cout << "Player 1 (Yellow) Won " << wins[1] << " games" << std::endl;
  std::cout << "Player 2 (Blue)   Won " << wins[2] << " games" << std::endl;
  std::cout << "Player 3 (Red)    Won " << wins[3] << " games" << std::endl;
}

void genetic_algorithm::init_population(){
  net.create_from_file("./../../ann_code/ludo_player.net");
  unsigned int num_connections = net.get_total_connections();
  struct fann_connection *connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  super_population.push_back( get_chromosome( connections, num_connections) );
  for (size_t i = 1; i < POP_SIZE; i++)
    super_population.push_back( add_gaussian_noise_to_chromosome( super_population[0]) );
}

void genetic_algorithm::evaluation(){

}
