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
    ludo_player_random p1_green;
    ludo_player_random p2_yellow;
    ludo_player_random p3_blue;
    super_ludo_player p4_red;

    game g;
    g.setGameDelay(0); //if you want to see the game, set a delay

    /* Add a GUI <-- remove the '/' to uncomment block
    Dialog w;
    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
    QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
    QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
    QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    w.show();
    /*/ //Or don't add the GUI
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    //*/

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

    for(int i = 0; i < 1000; ++i){
        g.start();
        a.exec();
        g.reset();
    }

    std::cout << "Player 0 (Green)  Won " << g.wins[0] << " games" << std::endl;
    std::cout << "Player 1 (Yellow) Won " << g.wins[1] << " games" << std::endl;
    std::cout << "Player 2 (Blue)   Won " << g.wins[2] << " games" << std::endl;
    std::cout << "Player 3 (Red)    Won " << g.wins[3] << " games" << std::endl;

    // g.start();
    // a.exec();

    return 0;
}
