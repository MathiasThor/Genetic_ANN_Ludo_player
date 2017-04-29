#include "neural_network.h"

int main(int argc, char *argv[]){
    QApplication* app = new QApplication( argc, argv );

    // genetic_algorithm GA(app, "generation_830.bin");
    //genetic_algorithm GA(app);
    neural_network NN(app);
    for (int i = 0; i < 10; i++) 
      NN.star_gamer();
    // NN.play();
    // NN.learn();
    return 0;
}


// Play:
// 2540
// 2612
// 2517
// 2331

// Star_gamer:
// 3108
// 2406
// 2261
// 2225
