//
// Created by Prannaya on 24/5/2022.
//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "img/img.h"
#include "library.h"


int main() {
    srand(time(NULL));

    //TRAINING
    int number_imgs = 10000;

    Img** imgs = csv_to_imgs("data/mnist_train.csv", number_imgs);

    int mark[] = {19, 10, 8, 17, 9};

    NeuralNetwork* net = network_create(784, mark, 6, 10, 0.01);
    network_train_batch_imgs(net, imgs, number_imgs);
    network_save(net, "testing_net");

    // imgs_free(imgs, number_imgs);

    // PREDICTING
    number_imgs = 3000;
    imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);
    net = network_load("testing_net");
    double score = network_predict_imgs(net, imgs, 1000);
    printf("Score: %1.5f", score);

    // imgs_free(imgs, number_imgs);
    // network_free(net);
    return 0;
}
