#include "neural_network.h"

int main(int argc, char *argv[]){
    QApplication* app = new QApplication( argc, argv );

    //genetic_algorithm GA(app, "generation_90.bin");
    // genetic_algorithm GA(app);
    neural_network NN(app);
    NN.star_gamer();
    return 0;
}


// Play:
// 2828
// 2545
// 2351
// 2276

// Star_gamer:
// 2866
// 2447
// 2504
// 2183
