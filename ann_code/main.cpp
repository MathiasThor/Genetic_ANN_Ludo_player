#include "fann.h"

// http://leenissen.dk/fann/wp/help/getting-started/
// Follow this example "bitch dick face"

int main()
{
    const unsigned int num_input = 52;
    const unsigned int num_output = 4;
    const unsigned int num_layers = 4;
    const unsigned int num_neurons_hidden = 16;
    const float desired_error = (const float) 0.01;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID);
    fann_set_activation_function_output(ann, FANN_SIGMOID);

    fann_train_on_file(ann, "plays.data", max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, "ludo_player.net");

    fann_destroy(ann);

    return 0;
}
