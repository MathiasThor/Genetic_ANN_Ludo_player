#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "super_ludo_player.h"
#include "ludo_player_random.h"
#include "positions_and_dice.h"

Q_DECLARE_METATYPE( positions_and_dice )

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();

    //instanciate the players here
    super_ludo_player p1_green;
    super_ludo_player p2_yellow;
    super_ludo_player p3_blue, p4_red;

    game g;
    g.setGameDelay(100); //if you want to see the game, set a delay

    //set up for each player
    QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1_green,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1_green,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1_green,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1_green,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2_yellow,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2_yellow,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2_yellow,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2_yellow,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3_blue,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3_blue,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3_blue,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3_blue,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4_red,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4_red,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4_red,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4_red,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    //* add a GUI
    Dialog w;
    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
    QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
    QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
    QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner(int)));
    w.show();
    //*/


    g.start();


    return a.exec();
}
