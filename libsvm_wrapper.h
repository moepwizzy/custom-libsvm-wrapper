#ifndef LIBSVM_WRAPPER_H
#define LIBSVM_WRAPPER_H

#include <vector>
#include <cstdlib>
#include "svm.h"


enum label_t {
    POS,
    NEG
};

struct Instance {
    std::vector<double> *vec;
    label_t label;
    int nonZeroEntries;
    Instance(std::vector<double> *v) : vec(v) {
        nonZeroEntries = 0;
        for (std::vector<double>::iterator it = v->begin(); it != v->end(); ++it)
            nonZeroEntries = *it == 0 ? nonZeroEntries : nonZeroEntries + 1;
    };
    Instance(std::vector<double> *v, label_t lab) : vec(v), label(lab) {
        nonZeroEntries = 0;
        for (std::vector<double>::iterator it = v->begin(); it != v->end(); ++it)
            nonZeroEntries = *it == 0 ? nonZeroEntries : nonZeroEntries + 1;
    };
};

inline void do_nothing(const char *s) {};

class libsvm {
    svm_model* model;
    svm_parameter* param;
    svm_problem* problem;
    bool canBeTrained;
    bool canBeTested;
    bool prepared;
    bool trained;
    bool generateParam();
    double* getWeights();
    std::vector<Instance*> trainVec;
    std::vector<Instance*> testVec;
    svm_node** finalTestVec;
public:
    libsvm(){
        param = new svm_parameter;
        problem = new svm_problem;
        svm_set_print_string_function(&do_nothing);
        canBeTested = false;
        canBeTrained = false;
        prepared = false;
        trained = false;
    };
    ~libsvm() {
        if (prepared)
            svm_destroy_param(param);
        if (trained)
            svm_free_model_content(model);
        for (std::vector<Instance*>::iterator it = trainVec.begin(); it != trainVec.end(); ++it)
            delete *it;
        for (std::vector<Instance*>::iterator it = testVec.begin(); it != testVec.end(); ++it)
            delete *it;
    };
    svm_parameter* getParameter() {return param;};
    bool prepare();
    bool addTrainSeq(std::vector<double>*, label_t);
    bool addTestSeq(std::vector<double>*, label_t);
    bool train();
    double evaluate();
};
#endif
