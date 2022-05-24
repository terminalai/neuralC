#include "library.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#define MAXCHAR 1000


double sigmoid(double input) {
    return 1.0 / (1 + exp(-input));
}

double crossEntropy(double input) {
    return input * (1 - input);
}

Matrix* sigmoidPrime(Matrix* m) {
    Matrix* newMatrix = apply(crossEntropy, m);
    return newMatrix;
}

Matrix* softmax(Matrix* m) {
    Matrix* ex = apply(exp, m);
    scale(1/sum(ex), ex);
    return ex;
}


NeuralNetwork* network_create(int input_size, int* hidden_sizes, int n_layers, int output_size, double lr) {
    NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
    net->input_size = input_size;
    net->hidden_sizes = hidden_sizes;
    net->n_layers = n_layers;
    net->output_size = output_size;
    net->learning_rate = lr;

    Layer** layers = malloc(n_layers * sizeof(Layer));

    int inputter = input_size;
    for(int i = 0; i < n_layers-1; i++) {
        Layer* layer = malloc(sizeof(Layer));
        layer->input_size = inputter;
        inputter = hidden_sizes[i];
        layer->output_size = inputter;
        Matrix* weights = matrix_create(inputter, layer->input_size);
        matrix_randomize(weights, inputter);
        layer->weights = weights;
        layers[i] = layer;
    }

    Layer* layer = malloc(sizeof(Layer));
    layer->input_size = inputter;
    layer->output_size = output_size;
    Matrix* weights = matrix_create(output_size, inputter);
    matrix_randomize(weights, output_size);
    layer->weights = weights;
    layers[n_layers-1] = layer;

    net->layers = layers;

    return net;
}

Matrix* network_predict(NeuralNetwork* net, Matrix* input_data) {
    Matrix* hidden_output = matrix_copy(input_data);
    for(int i = 0; i < net->n_layers; i++) {
        hidden_output = apply(sigmoid, dot(net->layers[i]->weights, hidden_output));
    }
    return hidden_output;
}

void network_train(NeuralNetwork* net, Matrix* input, Matrix* output) {
    Matrix** outputs = malloc(net->n_layers * sizeof(Matrix));
    Matrix* hidden_output = matrix_copy(input);
    for(int i = 0; i < net->n_layers; i++) {
        // printf("Weights size %d %d\n", net->layers[i]->weights->rows, net->layers[i]->weights->cols);
        // printf("Output size %d %d\n", hidden_output->rows, hidden_output->cols);
        // printf("hidden %d\n", i);
        hidden_output = apply(sigmoid, dot(net->layers[i]->weights, hidden_output));
        outputs[i] = hidden_output;
    }

    Matrix ** errors = malloc(net -> n_layers * sizeof(Matrix));
    // printf("output size %d %d\n", output->rows, output->cols);
    // printf("hidden_output size %d %d\n", hidden_output->rows, hidden_output->cols);
    Matrix* hidden_error = subtract(output, hidden_output);

    // printf("transposed outputs %d %d\n", outputs[net->n_layers-2]->cols, outputs[net->n_layers-2]->rows);

    net->layers[net->n_layers-1]->weights = add(
            net->layers[net->n_layers-1]->weights, // (10, 9)
            scale(
                    net->learning_rate,
                    dot(
                            multiply(
                                    hidden_error, // (10, 1)
                                    sigmoidPrime(output) // (10, 1)
                            ),
                            matrix_transpose(outputs[net->n_layers-2]) // (1, 9)
                    ) // (10, 9)
            ) // (10, 9)
    );

    // printf("Output backprop done\n");

    for(int i = net->n_layers-2; i >= 0; i--) {
        hidden_output = outputs[i];
        // printf("transposed weights %d %d\n", net->layers[i+1]->weights->cols, net->layers[i+1]->weights->rows);
        // printf("hidden error %d %d\n", hidden_error->rows, hidden_error->cols);
        // printf("hidden output %d %d\n", hidden_output->rows, hidden_output->cols);
        // printf("transposed outputs %d %d\n", (i == 0 ? input : outputs[i-1])->cols, (i == 0 ? input : outputs[i-1])->rows);

        hidden_error = dot(matrix_transpose(net->layers[i+1]->weights), hidden_error);


        net->layers[i]->weights = add(
                net->layers[i]->weights,
                scale(
                        net->learning_rate,
                        dot(
                                multiply(
                                        hidden_error,
                                        sigmoidPrime(hidden_output)
                                ),
                                matrix_transpose(i == 0 ? input : outputs[i-1])
                        )
                )
        );
    }
}


void network_train_batch_imgs(NeuralNetwork* net, Img** imgs, int batch_size) {
    for(int i = 0; i < batch_size; i++) {
        if(i % 100 == 0) printf("Img No. %d\n", i);
        Img* cur_img = imgs[i];
        Matrix* img_data = matrix_flatten(cur_img->img_data, 0);
        Matrix* output = matrix_create(10, 1);
        output->entries[cur_img->label][0] = 1;
        network_train(net, img_data, output);
        matrix_free(output);
        matrix_free(img_data);
    }
}

Matrix* network_predict_img(NeuralNetwork* net, Img* img) {
    Matrix* img_data = matrix_flatten(img->img_data, 0);
    Matrix* res = network_predict(net, img_data);
    matrix_free(img_data);
    return res;
}

double network_predict_imgs(NeuralNetwork* net, Img** imgs, int n) {
    int n_correct = 0;
    for(int i = 0; i < n; i++) {
        Matrix* prediction = network_predict_img(net, imgs[i]);
        if(matrix_argmax(prediction) == imgs[i]->label) n_correct++;
        matrix_free(prediction);
    }
    return 1.0 * n_correct / n;
}

void network_save(NeuralNetwork* net, char* file_string) {
    mkdir(file_string);
    chdir(file_string);
    FILE* descriptor = fopen("descriptor", "w");
    fprintf(descriptor, "%d\n", net->input_size);
    for(int i = 0; i < net->n_layers; i++) {
        fprintf(descriptor, "%d\n", net->hidden_sizes[i]);
    }
    fprintf(descriptor, "%d\n", net->output_size);
    fprintf(descriptor, "%0.4f\n", net->learning_rate);
    fclose(descriptor);
    int str_size = 8+(int)log10(net->n_layers);
    for(int i = 0; i < net->n_layers-1; i++) {
        char *filename = (char*)malloc(str_size * sizeof(char));
        sprintf(filename, "hidden %d", i);
        matrix_save(net->layers[i]->weights, filename);
    }
    matrix_save(net->layers[net->n_layers-1]->weights, "output");
    printf("Successfully written to '%s'\n", file_string);
    chdir("-");
}

NeuralNetwork* network_load(char* file_string) {
    char entry[MAXCHAR];
    chdir(file_string);
    FILE* descriptor = fopen("descriptor", "r");

    // first we need to get the size of the hidden sizes array
    int size = -3;
    while(feof(descriptor) != 1) {
        fgets(entry, MAXCHAR, descriptor);
        size++;
    }
    fclose(descriptor);

    descriptor = fopen("descriptor", "r");

    fgets(entry, MAXCHAR, descriptor);
    int input_size = atoi(entry);

    int* hidden_sizes = malloc(size * sizeof(int));

    for(int i = 0; i < size; i++) {
        fgets(entry, MAXCHAR, descriptor);
        int cur_size = atoi(entry);
        hidden_sizes[i] = cur_size;
    }

    fgets(entry, MAXCHAR, descriptor);
    int output_size = atoi(entry);

    fgets(entry, MAXCHAR, descriptor);
    double learning_rate = strtod(entry, NULL);

    fclose(descriptor);

    NeuralNetwork* net = network_create(input_size, hidden_sizes, size+1, output_size, learning_rate);

    int str_size = 8+(int)log10(net->n_layers);
    for(int i = 0; i < size; i++) {
        char *filename = (char*)malloc(str_size * sizeof(char));
        sprintf(filename, "hidden %d", i);
        net->layers[i]->weights = matrix_load(filename);
    }

    net->layers[size]->weights = matrix_load("output");
    chdir("-");
    return net;
}

void network_print(NeuralNetwork* net);
void network_free(NeuralNetwork* net);
