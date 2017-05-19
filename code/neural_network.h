#ifndef NEURAL_N_H
#define NEURAL_N_H
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

#include "genetic_algorithm.h"

class neural_network {

private:
    // CONSTANTS
    QApplication* a;

public:
    neural_network( QApplication* app );
    void play();
    void star_gamer();
    void learn();
};

#endif // NEURAL_N_H
