#include "genetic_algorithm.h"
using namespace std;

genetic_algorithm::genetic_algorithm(int new_argc, char *new_argv[], string load_this_population){
  argc = new_argc;
  argv = new_argv;
  int generation = 1;
  string filename = "error";
  population new_generation;
  std::vector<chromo_eval> evaluation_list;
  std::vector<chromosome> crossover_offsprings;
  std::vector<int> two_parents;

  ////////////////////////////////
  // INIT POPULATION W. GAUSSIAN
  ////////////////////////////////
  cout << "\n##\nGENERATION " << generation << "\n##" << endl;
  if (load_this_population == "NO") {
    init_population();
  } else{
    super_population = load_generation(load_this_population);
  }


  ////////////////////////////////
  // EVALUATION BUG (Dbus thing)
  ////////////////////////////////
  for (size_t i = 0; i < 20; i++) {
    evaluation_list = evaluation();
    cout << "Lowest  fitness: " << evaluation_list[0].fitness << endl;
    cout << "Largest fitness: " << evaluation_list[evaluation_list.size()-1].fitness << endl;
    // for (size_t i = 0; i < evaluation_list.size(); i++)
    //   cout << "Fitness: " << evaluation_list[i].fitness << endl;

  ////////////////////////////////
  // SELECTION
  ////////////////////////////////
    new_generation.clear();
    cout << "Selection and Replacement" << flush;
    for (int i = 0; i < POP_SIZE/2; i++) {
      two_parents = selection_turnament();
      // cout << "parent 1: " << two_parents[0] << endl;
      // cout << "parent 2: " << two_parents[1] << endl;

  ////////////////////////////////
  // RECOMBINATION
  ////////////////////////////////
      // TODO crossover & mutation rates!
      crossover_offsprings = crossover(super_population[two_parents[0]],super_population[two_parents[0]]);
      new_generation.push_back(mutation(crossover_offsprings[0]));
      new_generation.push_back(mutation(crossover_offsprings[1]));
      //new_generation.push_back(mutation(super_population[two_parents[0]]));
      //new_generation.push_back(mutation(super_population[two_parents[1]]));
      cout << "." << flush;
    }
    cout << endl;
    std::cout << "New Pop size: " << new_generation.size() <<   " and "<< new_generation[1].size()   <<'\n';
    std::cout << "Old Pop size: " << super_population.size() << " and "<< super_population[1].size() << '\n';

  ////////////////////////////////
  // REPLACEMENT
  ////////////////////////////////
    //super_population.clear();
    super_population = new_generation;
    generation++;
    cout << "\n##\nGENERATION " << generation << "\n##" << endl;
    if (generation%5 == 0) {
      filename = "./generation_data/generation_"+to_string(generation)+".bin";
      save_generation(super_population, filename);
    }
  }

  // TODO Now it is generational
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
  cout << "Evaluating Population"<< flush;

  vector<chromo_eval> evaluation_list;
  float fitness = 0;
  float *fitness_pointer = &fitness;
  chromo_eval tmp_chromo_eval;
  for (int i = 0; i < super_population.size(); i++) {
    cout << "." << flush;
    fitness = 0;

    //for (size_t k = 0; k < 100; k++)
    play_game(super_population[i], fitness_pointer);

    tmp_chromo_eval.chromo_number = i;
    tmp_chromo_eval.fitness = fitness;
    evaluation_list.push_back(tmp_chromo_eval);
  }
  cout << endl;
  sort(evaluation_list.begin(), evaluation_list.end());

  return evaluation_list;
}

std::vector<int> genetic_algorithm::selection_turnament(){
  //cout << "Turnament Time!" << endl;

  std::vector<int> two_parents;
  std::random_device seeder;
  std::mt19937 rng(seeder());
  std::uniform_int_distribution<int> gen(0, super_population.size()-1);
  std::vector<int> rng_list;
  int random_num;
  for (int i = 0; i < 4; i++) {
    while(std::find(rng_list.begin(), rng_list.end(), random_num) != rng_list.end())
      random_num = gen(rng);
    rng_list.push_back(random_num);
  }

  int parent = play_turnament(super_population[rng_list[0]], super_population[rng_list[1]], super_population[rng_list[2]], super_population[rng_list[3]]);
  two_parents.push_back(rng_list[parent]);

  rng_list.clear();

  for (int i = 0; i < 4; i++) {
    while(std::find(rng_list.begin(), rng_list.end(), random_num) != rng_list.end() || random_num == rng_list[parent])
      random_num = gen(rng);
    rng_list.push_back(random_num);
  }

  parent = play_turnament(super_population[rng_list[0]], super_population[rng_list[1]], super_population[rng_list[2]], super_population[rng_list[3]]);
  two_parents.push_back(rng_list[parent]);

  return two_parents;
}

std::vector<chromosome> genetic_algorithm::crossover(chromosome parent1, chromosome parent2){
  chromosome offspring_12, offspring_21;

  /* In this way we split it up for each weight */
  /* It may need to be more advanced, like an mask */
  for (int i = 0; i < parent1.size(); i+=2) {
    offspring_12.push_back(parent1[i]);
    offspring_12.push_back(parent2[i+1]);
    offspring_21.push_back(parent2[i]);
    offspring_21.push_back(parent1[i+1]);
  }

  std::vector<chromosome> v{offspring_12, offspring_21};
  return v;
}

chromosome genetic_algorithm::mutation(chromosome parent){

  // TODO Make the chance for mutation inverse propertional to the MAX fitness
  float chance_of_mutation = 0.5;

  std::random_device seeder;
  std::mt19937 rng(seeder());
  std::uniform_real_distribution<double> gen(0.0, 1.0);
  std::uniform_real_distribution<double> gen2(-2.0, +2.0);

  // https://www.ibm.com/support/knowledgecenter/ssw_aix_72/com.ibm.aix.progcomc/single_pre_float.htm
  // We only alter the fraction
  for (int i = 0; i < parent[i].size(); i++) {
    for (int j = 31; j < 9; j--) { // DOES NOT INCLUDE THE SIGN
      if (gen(rng) > chance_of_mutation) {
        parent[i].flip(j);
      }
    }
  }

  return parent;
}

void genetic_algorithm::save_generation(population pop_to_save, string filename){
  std::ofstream file(filename, std::ofstream::binary);
  unsigned long n;
  for(size_t i = 0; i < pop_to_save.size(); i++ ) {
    for (size_t j = 0; j < pop_to_save[i].size(); j++) {
      n = pop_to_save[i][j].to_ulong();
      file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    }
  }

  std::cout << "Saved population to: " << filename << '\n';
}

population genetic_algorithm::load_generation(string filename){
  std::ifstream file(filename, std::ofstream::binary);
  population loaded_pop (20, std::vector<bitset<32>> (394,0));
  unsigned long n;
  for(size_t i = 0; i < 20; i++ ) {
    for (size_t j = 0; j < 394; j++) {
      file.read( reinterpret_cast<char*>(&n), sizeof(n) );
      loaded_pop[i][j] = n;
    }
  }

  std::cout << "Loaded population from: " << filename << '\n';
  return loaded_pop;
}

// std::cout << "Player 0 (Green)  Won " << wins[0] << " games" << std::endl;
// std::cout << "Player 1 (Yellow) Won " << wins[1] << " games" << std::endl;
// std::cout << "Player 2 (Blue)   Won " << wins[2] << " games" << std::endl;
// std::cout << "Player 3 (Red)    Won " << wins[3] << " games" << std::endl;
