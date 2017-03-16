#include <stdio.h>
#include <iostream>
#include <vector>
#include "floatfann.h"
#include "fann.h"

using namespace std;
// http://leenissen.dk/fann/wp/help/getting-started/
// Follow this example "bitch dick face"

int main(int argc, char* argv[])
{
  string choise = argv[1];
  if (choise=="train") {
    cout << "========\nTRAINING\n========\n" << endl;
    const unsigned int num_input = 52;
    const unsigned int num_output = 4;
    const unsigned int num_layers = 4;
    const unsigned int num_neurons_hidden = 16;
    const float desired_error = (const float) 0.01;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID);
    fann_set_activation_function_output(ann, FANN_SIGMOID);

    fann_train_on_file(ann, "plays.data", max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, "ludo_player.net");

    fann_destroy(ann);
  }
  else if (choise=="test") {
    cout << "========\nTESTING\n========\n" << endl;
    fann_type *calc_out;
    fann_type input[52];

    // 1000
    std::vector<float> input_vec{0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    // 0100
    //std::vector<float> input_vec{0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0};
    // 0010
    //std::vector<float> input_vec{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1};
    // 0001
    //std::vector<float> input_vec{0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0};

    struct fann *ann = fann_create_from_file("ludo_player.net");

    for (int i = 0; i < input_vec.size(); i++) {
      input[i] = input_vec[i];
    }

    calc_out = fann_run(ann, input);

    printf("ludo player test -> %f %f %f %f\n", calc_out[0], calc_out[1], calc_out[2], calc_out[3]);

    fann_destroy(ann);
  }

    return 0;
}
