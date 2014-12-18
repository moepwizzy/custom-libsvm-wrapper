#include "libsvm_wrapper.h"

bool libsvm::prepare() {
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
    return generateParam();
}

bool libsvm::addTrainSeq(std::vector<int> vec, int label) {
    if (prepared)
        return false;
    trainVec.push_back(new Instance(vec,label));
    return true;
}

bool libsvm::addTestSeq(std::vector<int> vec) {
    if (prepared)
        return false;
    testVec.push_back(new Instance(vec));
    return true;
}

bool libsvm::generateParam() {
    if (!prepared)
        return false;
    param = new svm_parameter();
    param->svm_type = C_SVC;
    param->kernel_type = LINEAR;
    param->cache_size = 1024;
    param->C = 1;
    param->eps = 0.001;
    param->nr_weight = 2;
    param->weight_label = new int[2];
    param->weight_label[0] = -1;
    param->weight_label[1] = 1;
    param->weight = getWeights();
    param->probability = 1;
    return (canBeTrained = true);
}

double* libsvm::getWeights () {
    double* weights = new double[2];
    for (std::vector<Instance*>::iterator it = trainVec.begin(); it != trainVec.end(); ++it)
        (*it)->label == -1 ? weights[0]++ : weights[1]++;
    return weights;
}

bool libsvm::train() {
    if (!canBeTrained)
        return false;
    const char* str = svm_check_parameter(problem,param);
    if (str != NULL)
        return false;
    model = svm_train(problem,param);
    return true;
}

double libsvm::evaluate() {
    
}