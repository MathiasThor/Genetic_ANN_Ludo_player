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
  // EVALUATION BUG (Dbus thing)
  ////////////////////////////////
  std::vector<chromo_eval> evaluation_list = evaluation();

  ////////////////////////////////
  // SELECTION
  ////////////////////////////////
  // TODO - Maybe simply roulette - Solution on slide
  // TODO - Turnament seems simple as well.

  ////////////////////////////////
  // Recombination
  ////////////////////////////////
  // TODO Crossover - Every 2nd weight is from parent 1 and visa versa
  // TODO Mutation - Flip Bit
  // TODO Set crossover & mutation rates

  // TODO Consider steady state: Child replaces worst parent if child is better Or
  // TODO Elitism: generational, but best n parents survive

  ////////////////////////////////
  // STOPPING CRITERION
  ////////////////////////////////
  // TODO manual, after X generations
  // TODO Save every generation
    // TODO Fitness data
    // TODO Chromosomes, so we can start from a point again


  cout << "GENETIC ALGORITHM FINISHED!" << endl;
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

vector<chromo_eval> genetic_algorithm::evaluation(){
  vector<chromo_eval> evaluation_list;
  float fitness = 0;
  chromo_eval tmp_chromo_eval;
  cout << "Evaluating Population"<< flush;
  for (int i = 0; i < super_population.size(); i++) {
    cout << "." << flush;
    fitness = 0;

    //for (size_t k = 0; k < 100; k++)
    play_game(super_population[i], fitness);

    tmp_chromo_eval.chromo_number = i;
    tmp_chromo_eval.fitness = fitness;
    evaluation_list.push_back(tmp_chromo_eval);
  }
  cout << endl;
  sort(evaluation_list.begin(), evaluation_list.end()); // TODO FIX THIS
  return evaluation_list;
}

// std::cout << "Player 0 (Green)  Won " << wins[0] << " games" << std::endl;
// std::cout << "Player 1 (Yellow) Won " << wins[1] << " games" << std::endl;
// std::cout << "Player 2 (Blue)   Won " << wins[2] << " games" << std::endl;
// std::cout << "Player 3 (Red)    Won " << wins[3] << " games" << std::endl;
