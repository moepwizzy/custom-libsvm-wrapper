#ifndef LIBSVM_WRAPPER_H
#define LIBSVM_WRAPPER_H

#include "svm.h"
#include <vector>
#include <cstdlib>
void do_nothing(const char *s) {};

struct Instance {
    std::vector<int> vec;
    int label;
    Instance(std::vector<int> v) : vec(v) {};
    Instance(std::vector<int> v, int lab) : vec(v), label(lab) {};
};

class libsvm {
    svm_model* model;
    svm_parameter* param;
    svm_problem* problem;
    bool canBeTrained;
    bool canBeTested;
    bool prepared;
    bool generateParam();
    double* getWeights();
    std::vector<Instance*> trainVec;
    std::vector<Instance*> testVec;
public:
    libsvm(svm_model* mod) : model(mod) {
        svm_set_print_string_function(&do_nothing);
        canBeTested = true;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_parameter* par) : param(par) {
        svm_set_print_string_function(&do_nothing);
        canBeTested = false;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_problem* pro) : problem(pro) {
        svm_set_print_string_function(&do_nothing);
        canBeTested = false;
        canBeTrained = false;
        prepared = false;
    };
    libsvm(svm_parameter* par, svm_problem* pro) : param(par), problem(pro) {
        svm_set_print_string_function(&do_nothing);
        canBeTrained = true;
        canBeTested = false;
        prepared = false;
    };
    ~libsvm() {
        svm_destroy_param(param);
        svm_free_model_content(model);
        for (std::vector<Instance*>::iterator it = trainVec.begin(); it != trainVec.end(); ++it)
            delete (*it);
        for (std::vector<Instance*>::iterator it = testVec.begin(); it != testVec.end(); ++it)
            delete (*it);
    };
    svm_parameter* getParameter() {return param;};
    bool prepare();
    bool addTrainSeq(std::vector<int>, int);
    bool addTestSeq(std::vector<int>);
    bool train();
    double evaluate();
    double getAUC();
};

#endif
