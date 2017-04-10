#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T10:40:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ludo
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    game.cpp \
    ludo_player.cpp \
    super_ludo_player.cpp \
    super_ludo_player_state_functions.cpp \
    super_ludo_player_record_games.cpp \
    super_ludo_player_utils.cpp \
    genetic_algorithm.cpp \
    genetic_algorithm_utils.cpp \
    genetic_algorithm_game.cpp \
    neural_network.cpp \
    ludo_player_random.cpp

HEADERS  += dialog.h \
    game.h \
    ludo_player.h \
    positions_and_dice.h \
    super_ludo_player.h \
    genetic_algorithm.h \
    neural_network.h \
    ludo_player_random.h

FORMS    += dialog.ui

LIBS     += -lfann

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused
