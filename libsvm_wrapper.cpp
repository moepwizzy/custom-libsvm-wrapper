#include "libsvm_wrapper.h"

bool libsvm::prepare() {      
  problem->l = trainVec.size();
  problem->y = new double [trainVec.size()];
  problem->x = new svm_node* [trainVec.size()];
  for (unsigned int i = 0; i < trainVec.size(); ++i) {
    problem->y[i] = (double) trainVec.at(i)->label;
    problem->x[i] = new svm_node[trainVec.at(i)->nonZeroEntries+1];
    int nodeNumber = 0;
    for (int j = 0; j < trainVec.at(i)->nonZeroEntries; ++j) {
      if (trainVec.at(i)->vec->at(j) != 0) {
        problem->x[i][nodeNumber].index = j;
        problem->x[i][nodeNumber++].value = trainVec.at(i)->vec->at(j) / trainVec.at(i)->length;
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
        finalTestVec[i][nodeNumber++].value = testVec.at(i)->vec->at(j) / testVec.at(i)->length;
      }
    }
    finalTestVec[i][nodeNumber].index = -1;
  }
  prepared = true;
  return generateParam();
}

void libsvm::print_prepared_vectors() {
  for (int i = 0; i < problem->l; ++i) {
    int counter = 0;
    while(problem->x[i][counter].index != -1)
      std::cout<<problem->x[i][counter++].value<<"\t";
    std::cout<<std::endl<<std::endl;
  }
  for (std::size_t i = 0; i < testVec.size(); ++i) {
    int counter = 0;
    while(finalTestVec[i][counter].index != -1)
      std::cout<<finalTestVec[i][counter++].value<<"\t";
    std::cout<<std::endl<<std::endl;
  }
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
  param->degree = 0;
  param->cache_size = 1024;
  param->C = 1;
  param->eps = 0.001;
  param->nr_weight = 2;
  param->weight_label = new int[2];
  param->weight_label[0] = NEG;
  param->weight_label[1] = POS;
  param->weight = getWeights();
  param->probability = 1;
  param->shrinking = 0;
  return canBeTrained = true;
}

double* libsvm::getWeights () {
  double* weights = new double[2];
  for (std::vector<Instance*>::iterator it = trainVec.begin();
      it != trainVec.end(); ++it)
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
    print_param();
    std::cout<<str<<std::endl;
    return false;
  }
  model = svm_train(problem,param);
  return trained = true;
}

double libsvm::evaluate() {
  std::vector<double> pos;
  std::vector<double> neg;
  for (unsigned int i = 0; i < testVec.size(); ++i) {
    double estimates [2];
    svm_predict_probability(model, finalTestVec[i],estimates);
    if (testVec.at(i)->label == NEG)
      neg.push_back(estimates[0]);
    else
      pos.push_back(estimates[0]);
  }
  int sum = 0;
  for (vec_d_it_t posIt = pos.begin(); posIt != pos.end(); ++posIt)
    for (vec_d_it_t negIt = neg.begin(); negIt != neg.end(); ++negIt)
      sum += *posIt > *negIt ? 1 : 0;
  return (double)sum/(double)(pos.size()*neg.size());
}

void libsvm::print_param() {
  std::cout<<"printing parameter info"<<
    std::endl<<"param:type\t"<<param->svm_type<<
    std::endl<<"param:kernel\t"<<param->kernel_type<<
    std::endl<<"param:degree\t"<<param->degree<<
    std::endl<<"param:cache\t"<<param->cache_size<<
    std::endl<<"param:C\t"<<param->C<<
    std::endl<<"param:eps\t"<<param->eps<<
    std::endl<<"param:weight0\t"<<param->weight[0]<<
    std::endl<<"param:weight1\t"<<param->weight[1]<<
    std::endl<<"param:prob\t"<<param->probability<<
    std::endl<<"param:shrink\t"<<param->shrinking<<std::endl;
}
