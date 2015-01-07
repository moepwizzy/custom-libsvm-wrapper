/* 
 * File:   test.cpp
 * Author: marcus
 *
 * Created on January 6, 2015, 12:34 PM
 */

#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>

#include "libsvm_wrapper.h"

/*
 * 
 */
int main(int argc, char** argv) {
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> pos(0.4,0.1);
    std::normal_distribution<double> neg(0.6,0.1);
    libsvm test;
    for (int i = 0; i < 1000; ++i) {
        std::vector<double>* tmp_pos = new std::vector<double> {pos(generator),pos(generator)};
        std::vector<double>* tmp_neg = new std::vector<double> {neg(generator),neg(generator)};
        test.addTrainSeq(tmp_pos,POS);
        test.addTrainSeq(tmp_neg,NEG);
    }
    for (int i = 0; i < 100; ++i) {
        std::vector<double>* tmp_pos = new std::vector<double> {pos(generator),pos(generator)};
        std::vector<double>* tmp_neg = new std::vector<double> {neg(generator),neg(generator)};
        test.addTestSeq(tmp_pos,POS);
        test.addTestSeq(tmp_neg,NEG);
    }
    test.prepare();
    test.train();
    std::cout<<test.evaluate()<<std::endl;
    return 0;
}

