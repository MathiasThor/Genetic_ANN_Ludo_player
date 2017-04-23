#ifndef SUPER_LUDO_PLAYER_H
#define SUPER_LUDO_PLAYER_H
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
#include <random>
#include <chrono>

typedef std::vector< std::bitset<32> > chromosome;

class super_ludo_player : public QObject {
    Q_OBJECT
private:
    // CONSTANTS
    FANN::neural_net net;
    float* my_fitness;
    bool calculate_fitness;
    bool learn;

    // FUNCTIONS
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();

    bool can_kill(int pos, int new_dice_roll);
    bool can_get_home(int pos, int new_dice_roll);
    bool can_enter_safe_zone(int pos, int new_dice_roll);
    bool can_get_on_star(int pos, int new_dice_roll);
    bool can_get_on_globe(int pos, int new_dice_roll);
    bool can_enter_non_danger_zone(int pos, int new_dice_roll);
    bool can_get_killed(int pos, int new_dice_roll);
    bool can_get_out_of_start(int pos, int new_dice_roll);
    bool can_get_on_enemy_start(int pos, int new_dice_roll);
    bool can_move(int pos, int new_dice_roll);

    bool currently_on_enemy_start(int pos);
    bool currently_in_safe_zone(int pos);
    bool currently_in_non_danger_zone(int pos);
    bool currently_on_globe(int pos);
    bool currently_on_star(int pos);
    bool currently_home(int pos);

    bool enemy_globe(int pos);

    int record_my_games();

    void debug_stop(std::string action, int pos, bool cout_positions);
    float bitset_to_float(std::bitset<32>);
    std::vector<int> sorted_index(fann_type*);
    void set_chromosome_as_weights( chromosome input_chromo);
    void calc_fitness();
    union { float input; unsigned long long output;} data;

public:
    super_ludo_player(chromosome player_chromo, float *fitness);
    super_ludo_player(chromosome player_chromo);
    super_ludo_player(bool do_learning);
    super_ludo_player(bool do_learning, std::string load_from);
    ~super_ludo_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // SUPER_LUDO_PLAYER_H
