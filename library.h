#ifndef NEURALC_LIBRARY_H
#define NEURALC_LIBRARY_H

#include "linalg/matrix.h"
#include "img/img.h"

typedef struct {
    int input_size;
    int output_size;
    Matrix* weights;
} Layer;


typedef struct {
    int input_size;
    int* hidden_sizes;
    int n_layers;
    int output_size;
    double learning_rate;
    Layer** layers;
} NeuralNetwork;

double sigmoid(double input);
Matrix* sigmoidPrime(Matrix* m);
Matrix* softmax(Matrix* m);

NeuralNetwork* network_create(int input_size, int* hidden_sizes, int n_layers, int output_size, double lr);
void network_train(NeuralNetwork* net, Matrix* input_data, Matrix* output_data);
void network_train_batch_imgs(NeuralNetwork* net, Img** imgs, int batch_size);
Matrix* network_predict_img(NeuralNetwork* net, Img* img);
double network_predict_imgs(NeuralNetwork* net, Img** imgs, int n);
Matrix* network_predict(NeuralNetwork* net, Matrix* input_data);
void network_save(NeuralNetwork* net, char* file_string);
NeuralNetwork* network_load(char* file_string);
void network_print(NeuralNetwork* net);
void network_free(NeuralNetwork* net);

#endif //NEURALC_LIBRARY_H
