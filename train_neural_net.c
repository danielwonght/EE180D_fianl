#include "train_neural_net.h"

void train_neural_net(char *ifile_feature_name, char *net_name, int N_INPUT, int N_OUTPUT)
{
    /* train the model */
    const unsigned int num_input = N_INPUT;
    const unsigned int num_output = N_OUTPUT;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 9;
    const float desired_error = (const float) 0.0001;
    const unsigned int max_epochs = 50000;
    const unsigned int epochs_between_reports = 1000;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, ifile_feature_name, max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, net_name);

    fann_destroy(ann);
}


