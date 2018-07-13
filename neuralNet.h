#ifndef NEURALNET_H  
#define NEURALNET_H

#include <iomanip>
#include <iostream>
#include <vector>

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
      //NeuralNet();
      vector<vector<vector<double> > > getWeights() const {return weights;}
      NeuralNet crossover(NeuralNet, vector<unsigned int>);
      void setWeights(vector<vector<vector<double> > > newWeights){weights = newWeights;}
      void printWeights();
      double calculateNet(vector<double>);
      NeuralNet& operator=(const NeuralNet &rhs);
      double mutation(double);
      double sigmoid(double);
      vector<double> boardToInput(Board, player);
      // takes in another member of this class
      // returns a brand new neural net that is the offspring of the two parents
      // *this and the new one as a parameter by ref and const
      // Also need a calculate function
};

#endif
