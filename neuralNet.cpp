#include<vector>
#include<iostream>
#include <climits>
#include <stdlib.h> 
using namespace std;

int coinFlip();
double randomDouble();
// weight storage
// blondie24 page 41
class NeuralNet
{
   private:
   // first vector contains the number of layers.
   // deepest vector contains the weights
   //layer, neuron, weights
      vector<vector<vector<double> > > weights;
   public:
      NeuralNet(vector<int>);
      NeuralNet();
      vector<vector<vector<double> > > getWeights() {return weights;}
      NeuralNet* crossover(NeuralNet*&);
      void setWeights(vector<vector<vector<double> > > newWeights){weights = newWeights;}
      void printWeights(NeuralNet*&);
      // takes in another member of this class
      // returns a brand new neural net that is the offspring of the two parents
      // *this and the new one as a parameter by ref and const
      // Also need a calculate function
};
NeuralNet::NeuralNet()
{
   // hahaha idk man
}

// constructors

// size of the vector tells how many layers there are
// each number within tells the number of neurons in that layerSizes
// last number should always be one since we only want one output.
// layerSizes = (3, 2, 1)
// layer 1 has 2 nodes each with 3 inputs

NeuralNet::NeuralNet(vector<int> layerSizes)
{
   srandom(time(0)); 
   int i, j, k;
   vector<double> tempNeuron;
   vector<vector<double> > tempLayer;
   double randomNum;
   srandom(time(0)); // seed the random number generator.
   
   // Start at 1 since the first number in layerSizes only represents the number of inputs
   // The number i represents should be the number of neurons we are currently making.
   for(i = 1; i < layerSizes.size(); i++)
   {
      tempNeuron.clear();
      // next for loop should populate a neuron with weights
      // the number of weights is the previous value of layerSize
      for(j = 0; j < layerSizes.at(i - 1); j++)
      {
         randomNum = randomDouble();
         tempNeuron.push_back(randomNum);
      }
      //Our neuron is now full and ready to be added to the layer
      tempLayer.push_back(tempNeuron);
   }
   weights.push_back(tempLayer);
}

double randomDouble()
{
   int r = random();
   double randDouble = static_cast<double>(r) / static_cast<double>(INT_MAX);
   int coin;
   coin = coinFlip();
   if(coin == 1)
   {
      randDouble = randDouble * -1;
   }
   return randDouble;
}

int coinFlip()
{
   // Return a random integer between 0 and n - 1.
   int r;
   do
   {
      r = random();
   }
   while (r >= INT_MAX / 2 * 2);
   return r / (INT_MAX / 2);
}

int main()
{
   vector<int> layerSizesTest;
   layerSizesTest.push_back(4);
   layerSizesTest.push_back(3);
   layerSizesTest.push_back(1);
   NeuralNet *network;
   network = new NeuralNet(layerSizesTest);
   NeuralNet *mother = new NeuralNet(layerSizesTest);
   NeuralNet *father = new NeuralNet(layerSizesTest);
   printWeights(mother);
   printWeights(father);
   NeuralNet *child = mother->crossover(father);
   
   return 0;
}

NeuralNet* NeuralNet::crossover(NeuralNet*& mother)
{
   NeuralNet *father = this;
   vector<vector<vector<double> > > motherWeights = mother->getWeights();
   //The size of the first vector is the first number of a layerSize vector;
   vector<int> layerSizeOfParents;
   int i;
   /*cout << motherWeights.size() << endl;
   cout << motherWeights.at(0).size() << endl;
   cout << motherWeights.at(0).at(0).size() << endl;
   */
   // Try to determine the shape of the parent
   for(i = 0; i < motherWeights.at(0).size(); i++)
   {
      layerSizeOfParents.push_back(motherWeights.at(0).at(i).size());
   }
   // The last one should be one since we only use one output neuron
   layerSizeOfParents.push_back(1);
   /*for(i = 0; i < layerSizeOfParents.size(); i++)
   {
      cout << layerSizeOfParents.at(i) << " , ";
   }
   cout << endl;*/
   NeuralNet *child = new NeuralNet(layerSizeOfParents);
   
   return child;
}
void NeuralNet::printWeights(NeuralNet*& network)
{
   vector<vector<vector<double> > > testWeights;
   testWeights = network->getWeights();
   cout.precision(17);
   for(int i = 0; i < testWeights.size(); i++)
   {
      for(int j = 0; j < testWeights.at(i).size(); j++)
      {
         for (int k = 0; k < testWeights.at(i).at(j).size(); k++)
         {
            cout << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
}
// create a neuron that is a vector of doubles, push bakck one by one to create a layer. when the layer
// is done, push that back onto the big vector

// need crossover and mutation
// each weight flip a coin.

// code to evaluate the network
// code to test

// create net, print weights give inputs check by hand
// crossover or mutation functtion probably normally distributed
