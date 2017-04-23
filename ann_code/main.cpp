#include <stdio.h>
#include <iostream>
#include <vector>
#include "floatfann.h"
#include "fann_cpp.h"
#include <array>

using namespace std;
// http://leenissen.dk/fann/wp/help/getting-started/

int main(int argc, char* argv[])
{
  string choise = argv[1];
  if (choise=="train") {
    cout << "========\nTRAINING\n========\n" << endl;

    // SETUP NETWORK
    const unsigned int num_input = 60;
    const unsigned int num_output = 4;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 6;
    const float desired_error = (const float) 0.02;
    const float learning_rate = (const float) 0.8;
    const float init_weights_range = (const float) 0.77;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1;

    // CREATE NETWORK

    FANN::neural_net net;
    const std::array<uint, 4> layers = {num_input, num_neurons_hidden, num_output};
    net.create_standard_array(num_layers, layers.data());

    // SETUP NETWORK
    net.set_activation_function_hidden(FANN::SIGMOID);
    net.set_activation_function_output(FANN::SIGMOID);
    net.set_training_algorithm(FANN::TRAIN_INCREMENTAL);  // BATCH TRAIN MODE --> TRAIN_INCREMENTAL & TRAIN_BATCH
    net.set_learning_rate(learning_rate);
    net.randomize_weights(-init_weights_range, init_weights_range); // Suggested by Thimm and Fiesler
    // TODO include learning momentum? net.set_learning_momentum(SOME_FLOAT?);

    // GIVE TRAINING DATA AND START TRAINING
    net.train_on_file("./../data/plays_data/plays.data", max_epochs,
        epochs_between_reports, desired_error);

    // SAVE THE NETWORK AND DESTROY IT
    net.save("./../data/ludo_player.net");
    net.destroy();
  }
  else if (choise=="test") {
    cout << "========\nTESTING\n========\n" << endl;

    // MAKE INPUT AND OUTPUT LISTS
    fann_type *calc_out;
    fann_type input[60];

    // BUG OUTDATED
    // 1000
    //std::vector<float> input_vec{0,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
    // 0100
    std::vector<float> input_vec{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    // 0010
    //std::vector<float> input_vec{0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
    // 0001
    //std::vector<float> input_vec{0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0};

    // GET THE NETWORK
    FANN::neural_net net;
    net.create_from_file("./../data/ludo_player.net");

    // FILL UP WITH SOME INPUT
    for (int i = 0; i < input_vec.size(); i++) {
      input[i] = input_vec[i];
    }

    // CALCULATE OUTPUT FROM NETWORK
    calc_out = net.run(input);
    printf("ludo player test -> %f %f %f %f\n", calc_out[0], calc_out[1], calc_out[2], calc_out[3]);

    net.destroy();
  }
  else if (choise=="weights") {
    cout << "========\nWEIGHTS\n========\n" << endl;
    FANN::neural_net net;
    net.create_from_file("./../data/ludo_player.net");
    struct fann_connection 	*connections;
    unsigned int num_connections;

    num_connections = net.get_total_connections();
    connections = (struct fann_connection *) malloc(sizeof(struct fann_connection) * num_connections);
    net.get_connection_array(connections);

    for (int i = 0; i < num_connections; ++i) {
      printf("weight from %u to %u: %f\n", connections[i].from_neuron,
             connections[i].to_neuron, connections[i].weight);
    }
  }

    return 0;
}
