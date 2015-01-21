#include <math.h>

#include "libsvm_wrapper.h"

bool libsvm::prepare() {      
    problem->l = trainVec.size();
    problem->y = new double [trainVec.size()];
    problem->x = new svm_node* [trainVec.size()];
    for (unsigned int i = 0; i < trainVec.size(); ++i) {
        problem->y[i] = (double) trainVec.at(i)->label == POS ? 1 : -1;
        problem->x[i] = new svm_node[trainVec.at(i)->nonZeroEntries+1];
        int nodeNumber = 0;
        for (int j = 0; j < trainVec.at(i)->nonZeroEntries; ++j) {
            if (trainVec.at(i)->vec->at(j) != 0) {
                problem->x[i][nodeNumber].index = j;
                problem->x[i][nodeNumber++].value = trainVec.at(i)->vec->at(j);
            }
        }
        problem->x[i][nodeNumber].index = -1;
    }
    finalTestVec = new svm_node* [testVec.size()];
    for (unsigned int i = 0; i < testVec.size(); ++i) {
        finalTestVec[i] = new svm_node[testVec.at(i)->nonZeroEntries+1];
        int nodeNumber = 0;
        for (int j = 0; j < testVec.at(i)->nonZeroEntries; ++j) {
            if (testVec.at(i)->vec->at(j) != 0) {
                finalTestVec[i][nodeNumber].index = j;
                finalTestVec[i][nodeNumber++].value = testVec.at(i)->vec->at(j);
            }
        }
        finalTestVec[i][nodeNumber].index = -1;
    }
    prepared = true;
    return generateParam();
}

bool libsvm::addTrainSeq(std::vector<double> *vec, label_t lab) {
    if (prepared)
        return false;
    trainVec.push_back(new Instance(vec,lab));
    return true;
}

bool libsvm::addTestSeq(std::vector<double> *vec, label_t lab) {
    if (prepared)
        return false;
    testVec.push_back(new Instance(vec,lab));
    return true;
}

bool libsvm::generateParam() {
    if (!prepared) {
        std::cout<<"generateParam(): prepared==false"<<std::endl;
        return false;
    }
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
    return canBeTrained = true;
}

double* libsvm::getWeights () {
    double* weights = new double[2];
    for (std::vector<Instance*>::iterator it = trainVec.begin(); it != trainVec.end(); ++it)
        (*it)->label == NEG ? weights[0]++ : weights[1]++;
    return weights;
}

bool libsvm::train() {
    if (!canBeTrained) {
        std::cout<<"canBeTrained == false"<<std::endl;
        return false;
    }
    const char* str = svm_check_parameter(problem,param);
    if (str != NULL) {
        std::cout<<str<<std::endl;
        std::cout<<
          param->kernel_type<<std::endl<<
          param->svm_type<<std::endl;
        return false;
    }
    model = svm_train(problem,param);
    trained = true;
    return true;
}

double libsvm::evaluate() {
    std::vector<double> pos;
    std::vector<double> neg;
    for (unsigned int i = 0; i < testVec.size(); ++i) {
        double estimates [2];
        svm_predict_probability(model, finalTestVec[i],estimates);
        if (testVec.at(i)->label == NEG) {
            neg.push_back(estimates[0]);
        } else
            pos.push_back(estimates[0]);
    }
    int sum = 0;
    for (std::vector<double>::iterator posIt = pos.begin(); posIt != pos.end(); ++posIt)
        for (std::vector<double>::iterator negIt = neg.begin(); negIt != neg.end(); ++negIt)
            sum += *posIt > *negIt ? 1 : 0;
    return (double)sum/(double)(pos.size()*neg.size());
}
