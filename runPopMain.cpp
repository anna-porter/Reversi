#include "reversi.h"
#include "neuralNet.h"
#include "evolution.h"

using namespace std;

int main(int argc, char* argv[])
{
   int generations;
   vector<unsigned int> layerSizes;
   layerSizes.push_back(64);
   //layerSizes.push_back(8);
   layerSizes.push_back(1);
   Population initialPop, finalPop;
   generations = atoi(argv[1]);
   // Two arguments one represents a number of generations
   if(argc == 2)
   {
      initialPop = Population(layerSizes, 10);
      finalPop = initialPop.runGenerations(generations, layerSizes, 10, 0, "testing");
   }
   // Three arguments. one represents generations and the other represents a population file
   if(argc == 3)
   {
      string fileName = argv[2];
      initialPop = initialPop.loadPopulation(fileName);
      finalPop = initialPop.runGenerations(generations, layerSizes, 10, 0, "testing");
   }
   
}
