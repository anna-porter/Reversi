#ifndef EVOLUTION_H  
#define EVOLUTION_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <fstream>
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
      Organism();
      Organism(vector<vector<vector<double> > >);
      double getFitness() const {return fitness;}
      void updateFitness(int nextFitness) {fitness += nextFitness;}
      void setFitness(int newFitness) {fitness = newFitness;}
      vector<unsigned int> getLayerSizes() const{return layerSizes;}
      NeuralNet getNet() const {return brain;}
      void setNet(NeuralNet network) {brain = network;}
};
class Population
{
   private: 
      vector<vector<Organism> > pop;
      //int popSize;
      unsigned int numOfGenerations;
   public:
      Population(vector<unsigned int>, int);
      Population();
      Population(vector<vector<Organism> >);
      vector<vector<Organism> > getPop() const{return pop;}
      Organism getOrganism(int row, int col) const {return pop.at(row).at(col);}
      void setPop(vector<vector<Organism> > newPop) {pop = newPop;}
      vector<vector<int> > getAllFitnesses();
      void playNeighbors();
      pair<int, int> playGame(Organism, Organism);
      pair<int, int> playGamePrint(Organism, Organism);
      Population createNextGen();
      Population runGenerations(int, vector<unsigned int>, int, int, string);
      void printFitnesses();
      void assignFitnesses();
      void resetFitnesses();
      void printRep(string);
      Population loadPopulation(string);
      void savePopulation(int, int, string);
      void printPopulation();
      string popVSpop(Population);
      Population& operator=(const Population &rhs);
};
int reversiAgentOneMove(Board, player, Organism);
#endif
