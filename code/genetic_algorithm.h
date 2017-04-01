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

Q_DECLARE_METATYPE( positions_and_dice )

typedef std::vector< std::bitset<32> > chromosome;
typedef std::vector< std::vector< std::bitset<32> > > population;

struct chromo_eval {
  int chromo_number;
  float fitness;
  bool operator<(const chromo_eval& a) const{
      return fitness < a.fitness;
  }
} ;

class genetic_algorithm {

private:
    // CONSTANTS
    int argc;
    char **argv;
    const int POP_SIZE = 20;
    FANN::neural_net net;
    population super_population;

    // FUNCTIONS
    void debug_stop( std::string action );
    std::bitset<32> float_to_bitset(float);
    float bitset_to_float(std::bitset<32>);
    chromosome get_chromosome( fann_connection*, unsigned int );
    chromosome add_gaussian_noise_to_chromosome( chromosome input_chromo );
    void set_chromosome_as_weights( chromosome input_chromo);
    union { float input; unsigned long long output;} data;

    void init_population();
    std::vector<chromo_eval> evaluation();

public:
    genetic_algorithm(int argc, char *argv[]);
    std::vector<int> play_game(chromosome, float &fitness);

};

#endif // GENETIC_A_H