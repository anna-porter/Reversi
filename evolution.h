#ifndef EVOLUTION_H  
#define EVOLUTION_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <stdlib.h> 
#include "neuralNet.h"

using namespace std;

class Organism
{
   private:
      NeuralNet brain;
      int fitness;
      vector<unsigned int> layerSizes;
   public: 
      Organism(vector<unsigned int>);
      double getFitness() const {return fitness;}
      void updateFitness(int nextFitness) {fitness += nextFitness;}
      vector<unsigned int> getLayerSizes() const{return layerSizes;}
      NeuralNet getNet() const {return brain;}
      void setNet(NeuralNet network) {brain = network;}
};
class Population
{
   private: 
      vector<vector<Organism> > pop;
      int popSize;
      unsigned int numOfGenerations;
   public:
      Population(vector<unsigned int>, int);
      void playNeighbors();
      int getSize() {return popSize;}
      vector<vector<Organism> > getPop() const{return pop;};
      vector<vector<int> > getAllFitnesses();
      pair<int, int> playGame(Organism, Organism);
      Population createNextGen();
      Population runGenerations(int, vector<unsigned int>, int);
      void outputPopulation();
      void assignFittnesses();
      void recreatePopulation();
      void printPopulation();
      Population& operator=(const Population &rhs);
};
#endif
