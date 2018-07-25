#include "reversi.h"
#include "neuralNet.h" 
#include "evolution.h"
using namespace std;
//f(x) = x / (1 + abs(x))
int coinFlip();
double randomDouble();
int randomInt(int n);
//int reversiAgentOneMove(Board, player, NeuralNet&);
NeuralNet& NeuralNet::operator=(const NeuralNet &rhs)
{
   this->weights.clear();
   this->weights = rhs.getWeights();
   return *this;
}

NeuralNet::NeuralNet()
{
   this->weights.clear();
}

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
   //cout << "layerSIzes: " << layerSizes.size() << endl;
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
            //cout << i << " : " << tempLayer.size() << endl;
            //cout << i << " : " << tempLayer.size() << endl;
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

double NeuralNet::calculateNet(vector<double> input, ActFunc currentFunc)
{
   vector<vector<vector<double> > > currentWeights = this->getWeights();
   double summation;
   summation = 0;
   vector<double> lastInputs, nextInputs;
   unsigned int i, j, k;
   lastInputs = input;   
   // have to do minus one since we have a bias weight.;
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
               //cout << currentWeights.at(i).at(j).at(k) << " * " << lastInputs.at(k - 1) << " + " << endl;
               summation += currentWeights.at(i).at(j).at(k) * lastInputs.at(k - 1);
            }
         }
         //cout << currentWeights.at(i).at(j).at(0) << endl;
         // Add the bias.
         summation += currentWeights.at(i).at(j).at(0);
         // Use the sigmoid function.
         if(i != currentWeights.size() - 2)
         {
            //cout << "not last layer" << endl;
            summation = activation(summation, currentFunc);
            //cout << "Output = " << summation << endl;
         }
         nextInputs.push_back(summation);
         summation = 0;
      }
      lastInputs = nextInputs;
      //cout << lastInputs.size() << endl;
      nextInputs.clear();
   }  
   //cout << "final output = " << lastInputs.at(0) << endl;
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
            cout /*<< i << j << k << " : " */ << fixed << testWeights.at(i).at(j).at(k) << " , ";
         }
         cout << endl;
      }
      cout << endl;
   }
} 

void NeuralNet::saveWeights(ofstream &fout)
{
   vector<vector<vector<double> > > givenWeights;
   givenWeights = this->getWeights();
   //cout << givenWeights.size() << endl;
   //cout << givenWeights.at(0).size() << endl;
   //cout << givenWeights.at(0).at(0).size() << endl;
   cout.precision(17);
   unsigned int i, j, k;
   //int numweights = 0;
   for(i = 0; i < givenWeights.size(); i++)
   {
      fout << "newLayer" << endl;
      for(j = 0; j < givenWeights.at(i).size(); j++)
      {
         fout << "newNeuron" << endl;
         for (k = 0; k < givenWeights.at(i).at(j).size(); k++)
         {
            //numWeights++;
            fout << fixed << setprecision(17) << givenWeights.at(i).at(j).at(k) << ",";
         }
         fout << endl;
      }
   }
   //cout << "numWeights " <<  numWeights << endl;
   fout << "endOfOrganism" << endl;
}

double NeuralNet::activation(double x, ActFunc currentFunc)
{
   switch (currentFunc) 
   {
      case Sigmoid: return sigmoid(x); break;
      case Rectifier: return rectifier(x); break;
      case Softsign: return softsign(x); break;
      case Softplus: return softplus(x); break;
      case Threshold: return threshold(x); break;
   }
} 

double NeuralNet::mutation(double gene)
{
   randomMutation(gene);
}

double NeuralNet::randomMutation(double gene)
{
   if(randomInt(100) == 1)
   {
      //cout << "I mutated" << endl;
      gene = randomDouble();
   }
   return gene;
}

double NeuralNet::normalMutation(double gene)
{
   int adjust;
   default_random_engine generator(time(0));
   normal_distribution<double> distribution(0.0, 0.5);
   adjust = distribution(generator);
   return gene + adjust;
}

//new
double NeuralNet::softplus(double x)
{
   //cout << "using softplus" << endl;
   return log(1 + exp(x));
}
double NeuralNet::rectifier(double x)
{
   //cout << "using rectifier" << endl;
   if(x < 0)
   {
      return 0;
   }
   return x;
}

double NeuralNet::softsign(double x)
{
   //cout << "using softsign" << endl;
   return x / (1 + abs(x));
}

double NeuralNet::threshold(double x)
{
   //cout << "Using threshold" << endl;
   if (0 > x)
   {
      return 0;
   }
   return 1;
}
      
double NeuralNet::sigmoid(double x)
{
   //cout << "Using sigmoid" << endl;
   double eToTheX = exp(x);
   return (eToTheX / (eToTheX + 1));
}
