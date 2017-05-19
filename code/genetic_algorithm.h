#ifndef GENETIC_A_H
#define GENETIC_A_H
#include <QObject>
#include <iostream>
#include <random>
#include <game.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include "floatfann.h"
#include "fann_cpp.h"
#include "positions_and_dice.h"
#include <bitset>
#include <iterator>
#include <chrono>
#include "dialog.h"
#include <QApplication>

#include "ludo_player.h"
#include "super_ludo_player.h"
#include "ludo_player_random.h"
#include <algorithm>

Q_DECLARE_METATYPE( positions_and_dice )

typedef std::vector< std::bitset<32> > chromosome;
typedef std::vector< std::vector< std::bitset<32> > > population;

struct chromo_eval {
  int chromo_number;
  int wins;
  float fitness;
  bool operator<(const chromo_eval& a) const{
      return fitness < a.fitness;
  }
} ;

class genetic_algorithm {

private:
    // CONSTANTS
    QApplication* a;

    // GA-PARAMETERS
    const int POP_SIZE             = 30;
    const int PLAY_TIMES_EVAL      = 250;
    const int PLAY_TIMES_TURNAMENT = 250;
    const float GAUSSIAN_STDDEV    = 1.0;
    const float CROSSOVER_RATE     = 0.85;
    const float MUTATION_RATE      = 1.0; // Leave at 1
    const float MUTATION_PROB      = 0.45;
    const float MUTATION_STD       = 4.0; // Should be double the wanted amount
    chromo_eval current_best_eval;
    population current_best_chromo;
    FANN::neural_net net;

    std::random_device seeder;
    std::mt19937 rng;

    population super_population;

    // FUNCTIONS
    void debug_stop( std::string action );
    std::bitset<32> float_to_bitset(float);
    float bitset_to_float(std::bitset<32>);
    chromosome get_chromosome( fann_connection*, unsigned int );
    chromosome add_gaussian_noise_to_chromosome( chromosome input_chromo );
    void set_chromosome_as_weights( chromosome input_chromo);
    union { float input; unsigned long long output;} data;

    std::vector<int> play_turnament(chromosome player1_chromo, chromosome player2_chromo, chromosome player3_chromo, chromosome player4_chromo);

    void save_generation(population pop_to_save, std::string filename);
    population load_generation(std::string filename);
    void save_chromosome(chromosome pop_to_save, std::string filename);
    chromosome load_chromosome(std::string filename);

    void init_population();
    std::vector<chromo_eval> evaluation( population );
    std::vector<int> selection_turnament(std::vector<chromo_eval>);
    std::vector<int> selection_roulette(std::vector<chromo_eval>);
    std::vector<chromosome> crossover(chromosome parent1, chromosome parent2);
    std::vector<chromosome> crossover_2point(chromosome parent1, chromosome parent2);
    chromosome mutation(chromosome parent);

public:
    genetic_algorithm(QApplication* app , std::string load="NO");
    int play_game(chromosome, float *fitness);

};

#endif // GENETIC_A_H
