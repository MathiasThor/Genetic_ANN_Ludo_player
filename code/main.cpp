#include "neural_network.h"

int main(int argc, char *argv[]){
    QApplication* app = new QApplication( argc, argv );

    //genetic_algorithm GA(app, "generation_10.bin");
    genetic_algorithm GA(app);
    //neural_network NN(app);
    //NN.play();
    return 0;
}
