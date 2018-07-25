#ifndef NEURALNET_H  
#define NEURALNET_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <stdlib.h> 
enum ActFunc {Sigmoid, Rectifier, Softsign, Softplus, Threshold};

using namespace std;

class NeuralNet
{
   private:
   // between which neurons the weight is (layer)
   // what the final destination neuron is
   // what origin neuron it came from.
      vector<vector<vector<double> > > weights;
   public:
      NeuralNet(vector<unsigned int>);
      NeuralNet();
      NeuralNet(vector<vector<vector<double> > > newWeights) {weights = newWeights;}
      vector<vector<vector<double> > > getWeights() const {return weights;}
      NeuralNet crossover(NeuralNet, vector<unsigned int>);
      void setWeights(vector<vector<vector<double> > > newWeights){weights = newWeights;}
      void printWeights();
      void saveWeights(ofstream&);
      double calculateNet(vector<double>, ActFunc);
      NeuralNet& operator=(const NeuralNet &rhs);
      double mutation(double);
      double randomMutation(double);
      double normalMutation(double);
      double activation(double, ActFunc);
      double sigmoid(double);
      double softsign(double);
      double rectifier(double);
      double softplus(double);
      double threshold(double);
      
};

#endif
