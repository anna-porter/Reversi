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
   // between which neurons the weight is (layer)
   // what the final destination neuron is
   // what origin neuron it came from.
      vector<vector<vector<double> > > weights;
   public:
      NeuralNet(vector<unsigned int>);
      //NeuralNet();
      vector<vector<vector<double> > > getWeights() const {return weights;}
      NeuralNet* crossover(NeuralNet*&, vector<unsigned int>);
      void setWeights(vector<vector<vector<double> > > newWeights){weights = newWeights;}
      //void printWeights(NeuralNet*&);
      double calculateNet(NeuralNet*&, vector<double>);
      NeuralNet& operator=(const NeuralNet &rhs);
      // takes in another member of this class
      // returns a brand new neural net that is the offspring of the two parents
      // *this and the new one as a parameter by ref and const
      // Also need a calculate function
};

NeuralNet& NeuralNet::operator=(const NeuralNet &rhs)
{
   this->weights.clear();
   this->weights = rhs.getWeights();
   return *this;
}

/*
NeuralNet::NeuralNet()
{
   // hahaha idk man
}*/

// constructors

// size of the vector tells how many layers there are
// each number within tells the number of neurons in that layerSizes
// last number should always be one since we only want one output.
// layerSizes = (3, 2, 1)
// layer 1 has 2 nodes each with 3 inputs

NeuralNet::NeuralNet(vector<unsigned int> layerSizes)
{
 
   unsigned int i, destination, origin;
   vector<double> tempNeuron;
   vector<vector<double> > tempLayer;
   double randomNum;
   // loops the same number of times as the number of layers we have. 
   for(i = 0; i < layerSizes.size(); i++)
   {
      if(i + 1 < layerSizes.size())
      {
         // Loops the same number of times as the number of neurons the next layer has
         for(destination = 1; destination <= layerSizes.at(i + 1); destination++)
         {
            // Loops the same number of times as we had inputs from the previous layer
            // starting at 0 so we include the bias weights
            for(origin = 0; origin <= layerSizes.at(i); origin++)
            {
               randomNum = randomDouble();
               tempNeuron.push_back(randomNum);
            }
            tempLayer.push_back(tempNeuron);
            tempNeuron.clear();
         }
      }
      weights.push_back(tempLayer);
      tempLayer.clear();
   }
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
   srandom(time(0)); // seed the random number generator.
   vector<unsigned int> layerSizesTest;
   layerSizesTest.push_back(3);
   layerSizesTest.push_back(2);
   layerSizesTest.push_back(1);
   //NeuralNet *network;
   //network = new NeuralNet(layerSizesTest);

   NeuralNet *mother = new NeuralNet(layerSizesTest);
   NeuralNet *father = new NeuralNet(layerSizesTest);
   
   
   vector<vector<vector<double> > > testWeights;
   testWeights = mother->getWeights();
   cout << "Mother: " << endl;
   unsigned int i, j, k;
   cout.precision(17);
   for(i = 0; i < testWeights.size(); i++)
   {
      for(j = 0; j < testWeights.at(i).size(); j++)
      {
         for (k = 0; k < testWeights.at(i).at(j).size(); k++)
         {
            cout << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
   testWeights = father->getWeights();
   cout << "father: " << endl;
   cout.precision(17);
   for(i = 0; i < testWeights.size(); i++)
   {
      for(j = 0; j < testWeights.at(i).size(); j++)
      {
         for (k = 0; k < testWeights.at(i).at(j).size(); k++)
         {
            cout << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
   //this->printWeights(mother);
   //this->printWeights(father);
   //printWeights(mother);
   NeuralNet *child = mother->crossover(father, layerSizesTest);
   
   testWeights = child->getWeights();
   cout << "child: " << endl;
   cout.precision(17);
   for(i = 0; i < testWeights.size(); i++)
   {
      for(j = 0; j < testWeights.at(i).size(); j++)
      {
         for (k = 0; k < testWeights.at(i).at(j).size(); k++)
         {
            cout << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
   return 0;
}

NeuralNet* NeuralNet::crossover(NeuralNet*& mother, vector<unsigned int> parentShape)
{
   NeuralNet *father = this;
   vector<vector<vector<double> > > motherWeights = mother->getWeights();
   vector<vector<vector<double> > > fatherWeights = father->getWeights();

   unsigned int i, j, k;
   double gene;
   int coin;
   //motherWeights.size() is the size of layerSize when it created the net
   NeuralNet *child = new NeuralNet(parentShape);
   // ya just. like. ya fathah.
   child = father;
   for(i = 0; i < motherWeights.size(); i++)
   {
      for(j = 0; j < motherWeights.at(i).size(); j++)
      {
         for (k = 0; k < motherWeights.at(i).at(j).size(); k++)
         {
            coin = coinFlip();
            if(coin == 0)
            {
               cout << "I got it from mom" << endl;
               gene = motherWeights.at(i).at(j).at(k);
            }
            child->weights.at(i).at(j).at(k) = gene;
         }
      }
   }
   return child;
}
/*
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
}*/

double NeuralNet::calculateNet(NeuralNet*& network, vector<double> input)
{
   return 0;
}

// create a neuron that is a vector of doubles, push bakck one by one to create a layer. when the layer
// is done, push that back onto the big vector

// need crossover and mutation
// each weight flip a coin.

// code to evaluate the network
// code to test

// create net, print weights give inputs check by hand
// crossover or mutation functtion probably normally distributed
