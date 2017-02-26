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
    bool can_kill(int pos);
    bool can_get_home(int pos);
    bool can_enter_safe_zone(int pos);
    bool can_get_on_star(int pos);
    bool can_get_on_globe(int pos);
    bool can_enter_danger_zone(int pos);
    bool is_brick_in_safe_zone(int pos);

public:
    super_ludo_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_H
