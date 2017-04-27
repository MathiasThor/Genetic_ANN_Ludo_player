#include "genetic_algorithm.h"
using namespace std;

genetic_algorithm::genetic_algorithm( QApplication* app , string load_this_population):
  seeder(),
  rng(seeder())
  {
  current_best_eval.fitness = 0;
  current_best_eval.wins = 0;
  current_best_eval.chromo_number = 0;
  a = app;
  int generation = 1;
  string filename = "error";
  population new_generation;
  std::vector<chromo_eval> evaluation_list_current;
  std::vector<chromo_eval> evaluation_list_nxtgn;
  std::vector<chromosome> crossover_offsprings;
  std::vector<int> two_parents;
  std::uniform_real_distribution<double> gen(0.0, 1.0);
  float sum, sum2;

  ofstream average_file;
  average_file.open ("./../../data/average_fitness.dat");
  ofstream largest_file;
  largest_file.open ("./../../data/largest_fitness.dat");

  cout << "\nStarting Genetic Ludo Algorithm" << endl;
  cout << "-------------------------------" << endl;
  cout << "Population size:     \t" << POP_SIZE << endl;
  cout << "Games pr. evaluation:\t" << PLAY_TIMES_EVAL << endl;
  cout << "Games pr. turnament: \t" << PLAY_TIMES_TURNAMENT << endl;
  cout << "Gaussian std_dev:    \t" << GAUSSIAN_STDDEV << endl;
  cout << "Crossover Rate:      \t" << CROSSOVER_RATE << endl;
  cout << "Mutation Rate:       \t" << MUTATION_RATE << endl;
  cout << "Mutation Probability:\t" << MUTATION_PROB << endl;
  cout << "Mutation Amount:     \t" << MUTATION_STD << endl;
  cout << "-------------------------------\n" << endl;

  ////////////////////////////////
  // INIT POPULATION W. GAUSSIAN
  ////////////////////////////////

  if (load_this_population == "NO") {
    init_population();
  } else{
    unsigned first = load_this_population.find("_");
    unsigned last = load_this_population.find(".");
    string strNew = load_this_population.substr (first+1,last-first-1);
    super_population = load_generation("./../../data/generation_data/"+load_this_population);
    generation = atoi(strNew.c_str()) + 1;
  }
  cout << "\n----------------\n GENERATION " << generation << "\n----------------" << endl;


  ////////////////////////////////
  // EVALUATION
  ////////////////////////////////
  for (size_t i = 0; i < 10000; i++) {
    evaluation_list_current = evaluation(super_population);

    sum = 0;
    sum2 = 0;
    for (size_t i = 0; i < evaluation_list_current.size(); i++){
      sum += evaluation_list_current[i].wins;
      sum2 += evaluation_list_current[i].fitness;
    }

    // cout << "Lowest  Wins: " << (evaluation_list_current[0].wins/(double)PLAY_TIMES_EVAL)*100 << endl;
    // cout << "Largest Wins: " << (evaluation_list_current[evaluation_list_current.size()-1].wins/(double)PLAY_TIMES_EVAL)*100 << endl;
    // cout << "Avg.    Wins: " << ((sum/evaluation_list_current.size())/PLAY_TIMES_EVAL)*100 << endl;

    cout << "\nLowest  Fitness: " << evaluation_list_current[0].fitness << endl;
    cout << "Largest Fitness: " << evaluation_list_current[evaluation_list_current.size()-1].fitness << endl;
    cout << "Avg.    Fitness: " << (sum2/evaluation_list_current.size()) << endl;
    cout << "Win Ratio:       " << evaluation_list_current[evaluation_list_current.size()-1].wins << " of " << PLAY_TIMES_EVAL << endl << endl;


    largest_file << evaluation_list_current[evaluation_list_current.size()-1].fitness << endl;
    average_file << (sum2/evaluation_list_current.size()) << endl;

    // for (size_t i = 0; i < evaluation_list_current.size()/2; i++){
    //   cout << "fit.: " << evaluation_list_current[i].fitness << "     \t";
    //   cout << "fit.: " << evaluation_list_current[i+evaluation_list_current.size()/2].fitness << endl;
    // }

  ////////////////////////////////
  // SELECTION
  ////////////////////////////////
    new_generation.clear();
    cout << "Selection and Replacement" << flush;
    for (int i = 0; i < POP_SIZE/2; i++) {
      two_parents = selection_roulette(evaluation_list_current);
      // two_parents = selection_tournament(evaluation_list_current);

  ////////////////////////////////
  // RECOMBINATION
  ////////////////////////////////
      if (gen(rng) < CROSSOVER_RATE) {
        cout << ":" << flush;
        // crossover_offsprings = crossover(super_population[two_parents[0]],super_population[two_parents[1]]); // TODO -- TEST
        crossover_offsprings = crossover_2point(super_population[two_parents[0]],super_population[two_parents[1]]); // TODO -- TEST
      }
      else {
        cout << "." << flush;
        crossover_offsprings.push_back(super_population[two_parents[0]]);
        crossover_offsprings.push_back(super_population[two_parents[1]]);
      }

      if (gen(rng) < MUTATION_RATE) {
        cout << ";" << flush;
        new_generation.push_back(mutation(crossover_offsprings[0]));
        new_generation.push_back(mutation(crossover_offsprings[1]));
      }
      else {
        cout << "." << flush;
        new_generation.push_back(crossover_offsprings[0]);
        new_generation.push_back(crossover_offsprings[1]);
      }

      // for (size_t i = 0; i < new_generation[0][0].size(); i++) {
      //   std::cout << "Mutated: " << bitset_to_float(new_generation[0][i]) << '\n';
      //   std::cout << "Cleaned: " << bitset_to_float(super_population[0][i]) << '\n';
      // }
    }
    cout << endl;

  ////////////////////////////////
  // REPLACEMENT
  ////////////////////////////////
    // TODO Generational
    //super_population = new_generation;

    // TODO Elitism: generational, but best n parents survive
    evaluation_list_nxtgn = evaluation(new_generation);
    for (size_t i = 0; i < evaluation_list_nxtgn.size()/2; i++) {
      super_population[evaluation_list_current[i].chromo_number] = new_generation[evaluation_list_nxtgn[evaluation_list_nxtgn.size()-1-i].chromo_number];
    }

    // TODO Consider steady state: Child replaces worst parent if child is better Or
    // evaluation_list_nxtgn = evaluation(new_generation);
    // // // TODO SWAP THE WORST FIRST, NOT THE BEST
    // for (size_t i = evaluation_list_nxtgn.size()-1; i > 0 ; i--) {
    //   for (size_t j = evaluation_list_current.size()-1; j > 0; j--) {
    //     if (evaluation_list_nxtgn[i].fitness > evaluation_list_current[j].fitness) {
    //       //cout << "\nSwapped:" << endl;
    //       //cout << "Nxtgn: "<< evaluation_list_nxtgn[i].chromo_number << " Fitness: "   << evaluation_list_nxtgn[i].fitness << endl;
    //       //cout << "Curre: "<< evaluation_list_current[j].chromo_number << " Fitness: " << evaluation_list_current[j].fitness << endl;
    //       super_population[evaluation_list_current[j].chromo_number] = new_generation[evaluation_list_nxtgn[i].chromo_number];
    //       evaluation_list_current[j].fitness = evaluation_list_nxtgn[i].fitness;
    //       break;
    //     }
    //   }
    // }

    if (evaluation_list_current[evaluation_list_current.size()-1].wins > current_best_eval.wins) {
      current_best_eval.fitness = evaluation_list_current[evaluation_list_current.size()-1].fitness;
      current_best_eval.wins = evaluation_list_current[evaluation_list_current.size()-1].wins;
      current_best_eval.chromo_number = evaluation_list_current[evaluation_list_current.size()-1].chromo_number;
      cout << "New Best Chromosome: " << current_best_eval.chromo_number << endl;
      //filename = "best_chromosome.bin";
      //save_chromosome(super_population[current_best_eval.chromo_number], filename);
      set_chromosome_as_weights(super_population[current_best_eval.chromo_number]);
      net.save("./../../data/current_best_player.net");
    }

    generation++;
    cout << "\n----------------\n GENERATION " << generation << "\n----------------" << endl;
    // if (generation%5 == 0) {
    //   filename = "./../../data/generation_data/generation_"+to_string(generation)+".bin";
    //   save_generation(super_population, filename);
    // }
  }

  average_file.close();
  largest_file.close();
  cout << "GENETIC ALGORITHM FINISHED!" << endl;
}

void genetic_algorithm::init_population(){
  net.create_from_file("./../../data/ludo_player.net");
  unsigned int num_connections = net.get_total_connections();
  struct fann_connection *connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
  net.get_connection_array(connections);

  super_population.push_back( get_chromosome( connections, num_connections) );
  for (size_t i = 1; i < POP_SIZE; i++)
    super_population.push_back( add_gaussian_noise_to_chromosome( super_population[0]) );

  super_population[0] = add_gaussian_noise_to_chromosome(super_population[0]);
}

vector<chromo_eval> genetic_algorithm::evaluation(population input_pop){
  cout << "Evaluating Population"<< flush;

  vector<chromo_eval> evaluation_list;
  float fitness = 0;
  int wins = 0;
  float *fitness_pointer = &fitness;
  chromo_eval tmp_chromo_eval;
  for (int i = 0; i < input_pop.size(); i++) {
    cout << "." << flush;
    fitness = 0;
    wins = 0;

    wins = play_game(input_pop[i], fitness_pointer);

    tmp_chromo_eval.wins = wins;
    tmp_chromo_eval.chromo_number = i;
    tmp_chromo_eval.fitness = (((fitness*0.0001 + wins))/(double)PLAY_TIMES_EVAL) * 100; // TODO -- TEST (was 5)
    evaluation_list.push_back(tmp_chromo_eval);
  }
  cout << endl;
  sort(evaluation_list.begin(), evaluation_list.end());

  return evaluation_list;
}

std::vector<int> genetic_algorithm::selection_turnament(std::vector<chromo_eval> eval_list){
  int parent;
  //std::vector<int> winnings(4,0);
  std::vector<int> two_parents;
  std::random_device seeder;
  std::mt19937 rng(seeder());
  std::uniform_int_distribution<int> gen(super_population.size()*0.25, super_population.size()-1); // TODO -- TEST
  std::vector<int> rng_list;
  int random_num;
  for (int i = 0; i < 4; i++) {
    while(std::find(rng_list.begin(), rng_list.end(), random_num) != rng_list.end())
      random_num = gen(rng);
    rng_list.push_back(random_num);
  }

  std::vector<int> winnings = play_turnament(super_population[rng_list[0]], super_population[rng_list[1]], super_population[rng_list[2]], super_population[rng_list[3]]);

  int largest = 0;
  int largest_index = 0;
  for(int i = 0; i < winnings.size(); i++)
  {
      if( winnings[i] > largest ){
        largest=winnings[i];
        largest_index = i;
      }
  }

  two_parents.push_back(rng_list[largest_index]);
  rng_list.clear();
  winnings.clear();

  for (int i = 0; i < 4; i++) {
    while(std::find(rng_list.begin(), rng_list.end(), random_num) != rng_list.end() && random_num == rng_list[parent])
      random_num = gen(rng);
    rng_list.push_back(random_num);
  }

  winnings = play_turnament(super_population[rng_list[0]], super_population[rng_list[1]], super_population[rng_list[2]], super_population[rng_list[3]]);

  largest = 0;
  largest_index = 0;
  for(int i = 0; i < winnings.size(); i++)
  {
      if( winnings[i] > largest ){
        largest=winnings[i];
        largest_index = i;
      }
  }

  two_parents.push_back(rng_list[largest_index]);
  rng_list.clear();

  return two_parents;
}

std::vector<chromosome> genetic_algorithm::crossover(chromosome parent1, chromosome parent2){
  chromosome offspring_12, offspring_21;

  std::uniform_int_distribution<int> gen_cro(0.0, parent1.size());
  int cutspot = gen_cro(rng);

  for (int i = 0; i < cutspot; i++) {
    offspring_12.push_back(parent1[i]);
    offspring_21.push_back(parent2[i]);
  }

  for (int i = cutspot; i < parent2.size(); i++) {
    offspring_12.push_back(parent2[i]);
    offspring_21.push_back(parent1[i]);
  }

  std::vector<chromosome> v{offspring_12, offspring_21};
  return v;
}

std::vector<chromosome> genetic_algorithm::crossover_2point(chromosome parent1, chromosome parent2){
  chromosome offspring_12, offspring_21;

  std::uniform_int_distribution<int> gen_cro(0.0, parent1.size());
  int cutspot1 = gen_cro(rng);
  int cutspot2 = gen_cro(rng);

  if (cutspot1 > cutspot2) {
    std::swap(cutspot1,cutspot2);
  }

  for (int i = 0; i < cutspot1; i++) {
    offspring_12.push_back(parent1[i]);
    offspring_21.push_back(parent2[i]);
  }

  for (int i = cutspot1; i < cutspot2; i++) {
    offspring_12.push_back(parent1[i]);
    offspring_21.push_back(parent2[i]);
  }

  for (int i = cutspot2; i < parent2.size(); i++) {
    offspring_12.push_back(parent2[i]);
    offspring_21.push_back(parent1[i]);
  }

  std::vector<chromosome> v{offspring_12, offspring_21};
  return v;
}

chromosome genetic_algorithm::mutation(chromosome parent){

  float tmp = 0;

  std::uniform_real_distribution<double> gen(0.0, 1.0);
  std::uniform_real_distribution<float> gen1(-MUTATION_STD/2, MUTATION_STD/2);
  // std::normal_distribution<float> gen1(0, MUTATION_STD/2);

  for (int i = 0; i < parent.size(); i++) {
    if (gen(rng) < MUTATION_PROB) {
      tmp = bitset_to_float( parent[i] );
      tmp += gen1(rng);
      parent[i] = float_to_bitset( tmp );
    }
  }

  return parent;
}

std::vector<int> genetic_algorithm::selection_roulette(std::vector<chromo_eval> eval_list){

  vector<int> roulette_wheel;

  for (int i = 0; i < eval_list.size(); i++) {
    for (int j = 0; j < i; j++)
      roulette_wheel.push_back(eval_list[i].chromo_number);
  }

  // for (int i = 0; i < roulette_wheel.size(); i++) {
  //   cout << roulette_wheel[i] << " ";
  // }

  // cout << endl;

  std::uniform_int_distribution<int> select_parent(0, roulette_wheel.size()-1);
  vector<int> two_parents;

  two_parents.push_back(roulette_wheel[select_parent(rng)]);
  two_parents.push_back(roulette_wheel[select_parent(rng)]);

  // cout << two_parents[0] << " and " << two_parents[1] << endl;

  return two_parents;
}

// std::cout << "Player 0 (Green)  Won " << wins[0] << " games" << std::endl;
// std::cout << "Player 1 (Yellow) Won " << wins[1] << " games" << std::endl;
// std::cout << "Player 2 (Blue)   Won " << wins[2] << " games" << std::endl;
// std::cout << "Player 3 (Red)    Won " << wins[3] << " games" << std::endl;
