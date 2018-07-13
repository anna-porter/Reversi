#include<vector>
#include<iostream>
#include<climits>
#include<cstdlib> 
#include<cmath> 
#include "reversi.h"
#include "neuralNet.h" 

using namespace std;
//f(x) = x / (1 + abs(x))
int coinFlip();
double randomDouble();
int randomInt(int n);
int reversiAgentOneMove(Board reversiBoard, player mover, NeuralNet network);
// weight storage
// blondie24 page 41

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
   double randDouble = static_cast<double>(r) / INT_MAX;
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
   int r;
   do
   {
      r = random();
   }
   while (r >= INT_MAX / 2 * 2);
   return r / (INT_MAX / 2);
}

int randomInt(int n)
{
   int r;
   do
   {
      r = random();
   }
   while (r >= INT_MAX / n * n);
   return r / (INT_MAX / n);
}

int main()
{
   srandom(time(0)); // seed the random number generator.
   vector<unsigned int> layerSizesTest;
   //layerSizesTest.push_back(4);
   layerSizesTest.push_back(3);
   layerSizesTest.push_back(2);
   layerSizesTest.push_back(1);
   //NeuralNet *network;
   //network = new NeuralNet(layerSizesTest);

   NeuralNet mother = NeuralNet(layerSizesTest);
   NeuralNet father = NeuralNet(layerSizesTest);
   
   
   vector<vector<vector<double> > > testWeights;
   testWeights = mother.getWeights();
   cout << "Mother: " << endl;
   mother.printWeights();
   testWeights = father.getWeights();
   cout << "father: " << endl;
   father.printWeights();
   NeuralNet child = father.crossover(mother, layerSizesTest);
   cout << "child: " << endl;
   child.printWeights();
   vector<double> input;
   //input.push_back(1);
   input.push_back(1);
   input.push_back(2);
   input.push_back(3);
   //input.push_back(-1);
   cout << child.calculateNet(input) << endl;
   
   return 0;
}

double NeuralNet::calculateNet(vector<double> input)
{
   vector<vector<vector<double> > > currentWeights = this->getWeights();
   double summation;
   summation = 0;
   vector<double> lastInputs, nextInputs;
   unsigned int i, j, k;
   lastInputs = input;   
   // have to do minus one since we have a bias weight.
   if(input.size() != currentWeights.at(0).at(0).size() - 1)
   {
      cerr << "CALCULATENET: INPUT NUMBER DOES NOT MATCH GIVEN NET" << endl;
      exit(1);
   }
   for(i = 0; i < currentWeights.size() - 1; i++)
   {
      for(j = 0; j < currentWeights.at(i).size(); j++)
      {
         for (k = 1; k < currentWeights.at(i).at(j).size(); k++)
         {
            if(k - 1 >= 0)
            {
               cout << currentWeights.at(i).at(j).at(k) << " * " << lastInputs.at(k - 1) << " + " << endl;
               summation += currentWeights.at(i).at(j).at(k) * lastInputs.at(k - 1);
            }
         }
         cout << currentWeights.at(i).at(j).at(0) << endl;
         // Add the bias.
         summation += currentWeights.at(i).at(j).at(0);
         // Use the sigmoid function.
         if(i != currentWeights.size() - 2)
         {
            //cout << "not last layer" << endl;
            summation = sigmoid(summation);
            cout << "Output = " << summation << endl;
         }
         nextInputs.push_back(summation);
         summation = 0;
      }
      lastInputs = nextInputs;
      //cout << lastInputs.size() << endl;
      nextInputs.clear();
   }  
   cout << "final output = " << lastInputs.at(0) << endl;
   return lastInputs.at(0);
}

// Do cross over first and then go back through and mutate them
NeuralNet NeuralNet::crossover(NeuralNet mother, vector<unsigned int> parentShape)
{
   NeuralNet father = *this;
   vector<vector<vector<double> > > motherWeights = mother.getWeights();
   vector<vector<vector<double> > > fatherWeights = father.getWeights();

   unsigned int i, j, k;
   double gene;
   gene = 0;
   int coin;
   // ya just. like. ya fathah.
   NeuralNet child = father;
   for(i = 0; i < motherWeights.size(); i++)
   {
      for(j = 0; j < motherWeights.at(i).size(); j++)
      {
         for (k = 0; k < motherWeights.at(i).at(j).size(); k++)
         {
            coin = coinFlip();
            if(coin == 0)
            {
               //cout << "I got " << i << j << k << " from mom" << endl;
               //cout << motherWeights.at(i).at(j).at(k) << endl;
               gene = motherWeights.at(i).at(j).at(k);
               child.weights.at(i).at(j).at(k) = gene;
            }
         }
      }
   }
   for(i = 0; i < motherWeights.size(); i++)
   {
      for(j = 0; j < motherWeights.at(i).size(); j++)
      {
         for (k = 0; k < motherWeights.at(i).at(j).size(); k++)
         {
            gene = child.weights.at(i).at(j).at(k);
            child.weights.at(i).at(j).at(k) = mutation(gene);
         }
      }
   }
   
   return child;
}

void NeuralNet::printWeights()
{
   vector<vector<vector<double> > > testWeights;
   testWeights = this->getWeights();
   cout.precision(17);
   unsigned int i, j, k;
   for(i = 0; i < testWeights.size(); i++)
   {
      for(j = 0; j < testWeights.at(i).size(); j++)
      {
         for (k = 0; k < testWeights.at(i).at(j).size(); k++)
         {
            cout << i << j << k << " : "  << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
}

double NeuralNet::mutation(double gene)
{
   if(randomInt(100) == 1)
   {
      cout << "I mutated" << endl;
      gene = randomDouble();
   }
   return gene;
}

double NeuralNet::sigmoid(double x)
{
   double eToTheX = exp(x);
   return (eToTheX / (eToTheX + 1));
}

vector<double> NeuralNet::boardToInput(Board reversiBoard, player mover)
{
   int size, i;
   size = reversiBoard.getSize();
   Tile locationTile;
   vector<double> input;
   input.clear();
   for(i = 0; i < size * size; i++)
   {
      locationTile = reversiBoard.getTile(i);
      if(locationTile.getOwner() == none)
      {
         input.push_back(0);
      }
      else if (locationTile.getOwner() == mover)
      {
         input.push_back(1);
      }
      else
      {
         input.push_back(-1);
      }
   }
   return input;
}


int reversiAgentOneMove(Board reversiBoard, player mover, NeuralNet network)
{
   vector<double> inputs;
   Board copyBoard;
   int size, i, bestMove;
   double maxHeuristic, heuristic;
   maxHeuristic = -1000;
   bestMove = -1;
   heuristic = 0;
   size = reversiBoard.getSize();
   for(i = 0; i < size * size; i++)
   {
      copyBoard = reversiBoard;
      copyBoard.makeMove(i, mover);
      inputs = network.boardToInput(copyBoard, mover);
      heuristic = network.calculateNet(inputs);
      if(heuristic > maxHeuristic)
      {
         maxHeuristic = heuristic;
         bestMove = i;
      }
   }
   if(bestMove != -1)
   {
      return bestMove;
   }
   return bestMove; // return something else maybe?
}

// crossover or mutation functtion probably normally distributed


   // test reversi code. invalid
   // test crossover and mutate.
   // finsih and test calculateNet
// Writing as many mains for testng is fine
// read some more
// Have a function member function that takes a board as input and seed inputs. evaluate that board by calling calcNet - 1 0 and 1
// evaluate each available position, and use that to decide which move to make
// agent function that looks one move ahead and returns a move;
// thinking about the evolution code.
      // keeping track of populations of agents
      // how big pop
      // agents play against eachother to determine fitness
      // maybe score contributes
      // scoreboards locally
      // do they play against everyone or play against neighbors
      // making a taurus - top wraps and sides wraps
      // 6 direction neighbors perhaps
      // each play as black and as white
      
      // think about genetic algorithm stuff.
      
      // The old organism at this location, choses the neighbor with the "highest" fitness
      // More likely to choose the best one but still some likelihood towards the others.
      
       
// Get started on genetic algorithm
// Finish and test board to input and agent one move
