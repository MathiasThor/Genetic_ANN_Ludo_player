#include <stdio.h>
#include <iostream>
#include <vector>
#include "floatfann.h"
#include "fann_cpp.h"
#include <array>
#include <fstream>

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
    const float desired_error = (const float) 0.01;
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

    // GIVE TRAINING DATA AND START TRAINING
    net.train_on_file("./../data/plays_data/backup_5plays.data", max_epochs,
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
  else if (choise=="vali") {
    cout << "========\nVALIDATING\n========\n" << endl;
    FANN::neural_net net;
    net.create_from_file("./../data/ludo_player_test.net");
    struct fann_train_data *data = fann_read_train_from_file("./../data/plays_data/validation.data");
    net.reset_MSE();
    for(int i = 0 ; i != data->num_data ; i++ ) {
      net.test(data->input[i], data->output[i]);
    }
    printf("Mean Square Error: %f\n", net.get_MSE());
    fann_destroy_train(data);
  }
  else if (choise=="trainvali") {
    cout << "========\nTRAIN_VALIDATING\n========\n" << endl;
    FANN::training_data test;
    test.read_train_from_file("./../data/plays_data/backup_10plays.data"); // PATH TO TRAIN DATA HERE
    FANN::training_data vali;
    vali.read_train_from_file("./../data/plays_data/backup_5plays.data");  // PATH TO VALIDATION DATA HERE

    // SETUP NETWORK
    const unsigned int num_input = 60;
    const unsigned int num_output = 4;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 6;
    const float desired_error = (const float) 0.03;
    const float learning_rate = (const float) 0.8;
    const float init_weights_range = (const float) 0.77;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1;

    // CREATE NETWORK

    FANN::neural_net net;
    FANN::neural_net valinet;
    const std::array<uint, 4> layers = {num_input, num_neurons_hidden, num_output};
    net.create_standard_array(num_layers, layers.data());

    // SETUP NETWORK
    net.set_activation_function_hidden(FANN::SIGMOID);
    net.set_activation_function_output(FANN::SIGMOID);
    net.set_training_algorithm(FANN::TRAIN_BATCH);  // BATCH TRAIN MODE --> TRAIN_INCREMENTAL & TRAIN_BATCH
    net.set_learning_rate(learning_rate);
    net.randomize_weights(-init_weights_range, init_weights_range); // Suggested by Thimm and Fiesler

    std::ofstream test_log;
    test_log.open ("./train_vali.dat");

    // GIVE TRAINING DATA AND START TRAINING
    double error = 0;
    for(int i = 1 ; i <= max_epochs ; i++) {
      error = net.train_epoch(test);
      printf("Training Error: %f", error); // Training error
      if ( error < desired_error ) {
        break;
      }

      net.reset_MSE();
      for(int i = 0 ; i != vali.length_train_data() ; i++ ) {
        net.test(vali.get_input()[i], vali.get_output()[i]);
      }
      test_log << i << "\t" << error << "\t" << net.get_MSE() << "\n";
      printf("  \tValidation Error: %f\n", net.get_MSE()); // Validation error
    }

    // SAVE THE NETWORK AND DESTROY IT
    net.save("./../data/ludo_player.net");
    net.destroy();
    test_log.close();
  }

    return 0;
}
