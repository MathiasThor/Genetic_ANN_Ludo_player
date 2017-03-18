#ifndef SUPER_LUDO_PLAYER_H
#define SUPER_LUDO_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

class super_ludo_player : public QObject {
    Q_OBJECT
private:
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

    bool currently_on_enemy_start(int pos);
    bool currently_in_safe_zone(int pos);
    bool currently_in_non_danger_zone(int pos);
    bool currently_on_globe(int pos);
    bool currently_home(int pos);

    bool enemy_globe(int pos);

    int record_my_games();
    void debug_stop(std::string action, int pos, bool cout_positions);

public:
    super_ludo_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // SUPER_LUDO_PLAYER_H
