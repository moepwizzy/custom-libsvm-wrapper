#include "libsvm_wrapper.h"

void libsvm::prepare() {
    problem->l = trainVec.size();
    problem->y = new double [trainVec.size()];
    problem->x = new svm_node* [trainVec.size()];
    for (unsigned int i = 0; i < trainVec.size(); ++i) {
        problem->y[i] = (double) trainVec.at(i)->label;
        for (unsigned int j = 0; j < trainVec.at(i)->vec.size(); ++j) {
            problem->x[i][j].index = j;
            problem->x[i][j].value = trainVec.at(i)->vec.at(j);
        }
    }
    prepared = true;
}

void libsvm::addTrainSeq(std::vector<int> vec, int label) {
    trainVec.push_back(new Instance(vec,label));
}

void libsvm::addTestSeq(std::vector<int> vec) {
    testVec.push_back(new Instance(vec));
}

