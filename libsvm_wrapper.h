#ifndef LIBSVM_WRAPPER_H
#define LIBSVM_WRAPPER_H

#include "svm.h"
#include <vector>

struct Instance {
    std::vector<int> vec;
    int label;
    Instance(std::vector<int> v) : vec(v) {};
    Instance(std::vector<int> v, int lab) : vec(v), label(lab) {};
};

class libsvm {
    svm_model model;
    svm_parameter* param;
    svm_problem* problem;
    bool canBeTrained;
    bool canBeTested;
    bool prepared;
    bool generateParam();
    std::vector<Instance*> trainVec;
    std::vector<Instance*> testVec;
public:
    libsvm(svm_model mod) : model(mod) {
        canBeTested = true;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_parameter* par) : param(par) {
        canBeTested = false;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_problem* pro) : problem(pro) {
        canBeTested = false;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_parameter* par, svm_problem* pro) : param(par), problem(pro) {
        canBeTrained = true;
        canBeTested = false;
        prepared = false;
    };
    ~libsvm() {
        svm_destroy_param(param);
        svm_free_model_content(&model);
        free(trainVec);
        free(testVec);
    };
    void prepare();
    void addTrainSeq(std::vector<int>, int);
    void addTestSeq(std::vector<int>);
};


#endif
