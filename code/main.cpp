#include "neural_network.h"

int main(int argc, char *argv[]){
    QApplication* app = new QApplication( argc, argv );

    // genetic_algorithm GA(app, "generation_225.bin"); // Train from binary save
    // genetic_algorithm GA(app);                       // Train GA from start
    neural_network NN(app);                             // Make ANN player
    for (int i = 0; i < 10; i++)
      NN.star_gamer();                                  // Play with .NET vs. other players
    // for (int i = 0; i < 5; i++)
    //   NN.play();                                     // Play with ANN vs. SemiSmart
    // NN.learn();                                      // Make training data for ANN player
    return 0;
}
