#include "neural_network.h"

int main(int argc, char *argv[]){
    QApplication* app = new QApplication( argc, argv );

    // genetic_algorithm GA(app, "generation_355.bin"); // Train from binary save
    genetic_algorithm GA(app);                          // Train GA from start
    // neural_network NN(app);                          // Make ANN player
    // NN.star_gamer();                                 // Play with .NET vs. other players
    // NN.play();                                       // Play with ANN vs. SemiSmart
    // NN.learn();                                      // Make training data for ANN player
    return 0;
}
