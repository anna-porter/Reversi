#ifndef NEURALNET_H  
#define NEURALNET_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <stdlib.h> 

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
      void prettyPrintWeights();
      void saveWeights(ofstream&);
      double calculateNet(vector<double>);
      NeuralNet& operator=(const NeuralNet &rhs);
      double mutation(double);
      double activation(double);
      double sigmoid(double);
      double softsign(double);
      double rectifier(double);
      double softplus(double);
      double threshold(double);
      
};

#endif
