#include "reversi.h"
#include "neuralNet.h" 
#include "evolution.h"
#include <time.h>
#include <random>
#include <cmath>
#include <cfloat>
using namespace std;
//f(x) = x / (1 + abs(x))
int coinFlip();
double randomDouble();
default_random_engine generator(time(0));
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
               randomNum = normalShake(0);
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
   double randDouble = (static_cast<double>(r) / INT_MAX) * sqrt(3);
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
   if(n != 0)
   {
      int r;
      do
      {
         r = random();
      } while (r >= INT_MAX / n * n);
      
      return r / (INT_MAX / n);
   }
   return n;
}

 double NeuralNet::calculateNet(vector< double> input, ActFunc currentFunc)
{
   double summation, temp;
   //vector<vector<vector<double> > > currentWeights = this->getWeights();
   summation = 0;
   //int calcs = 0;
   vector< double> lastInputs, nextInputs;
   unsigned int i, j, k;
   lastInputs = input;
   //stringstream errorString;
   // have to do minus one since we have a bias weight.;
   if(input.size() != this->weights.at(0).at(0).size() - 1)
   {
      cerr << "CALCULATENET: INPUT NUMBER DOES NOT MATCH GIVEN NET" << endl;
      exit(1);
   }
   for(i = 0; i < this->weights.size() - 1; i += 1)
   {
      for(j = 0; j < this->weights.at(i).size(); j += 1)
      {
         for (k = 1; k < this->weights.at(i).at(j).size(); k += 1)
         {
            //cout << i << j << k << endl;
            if(k - 1 >= 0)
            {
               temp = this->weights.at(i).at(j).at(k) * lastInputs.at(k - 1);
               if(lastInputs.at(k-1) == 1)
               {
                  //errorString << this->weights.at(i).at(j).at(k) << "+";
               }
               else
               {
                  //errorString << this->weights.at(i).at(j).at(k) << "*" << lastInputs.at(k - 1) << " + ";
               }
               summation += temp;
               //calcs++;
            }
         }
         // Add the bias.
         summation += this->weights.at(i).at(j).at(0);
         //errorString << this->weights.at(i).at(j).at(0) << "\n";
         //calcs++;
         // Use the activation function.
         if(i != this->weights.size() - 2)
         {
            //errorString << "Summation after act " << summation << "\n";
            summation = activation(summation, currentFunc);
            //cout << "Summation after act = " << summation << endl;
            
         }
         //cout << summation << endl;
         nextInputs.push_back(summation);
         summation = 0;
      }
      lastInputs = nextInputs;
      nextInputs.clear();
   }
   //cout << "Network calcs: " << calcs << endl;
   if(isnan(lastInputs.at(0)))
   {
      //cout << errorString.str();
      cerr << "NaN value inside CalcNet" << endl;
      exit(4);
   }
   if(isinf(lastInputs.at(0)))
   {
      //cout << errorString.str();
      cerr << "inf value inside calcNet" << endl;
      exit(5);
   }
   return lastInputs.at(0);   
   
}

// Do cross over first and then go back through and mutate them
NeuralNet NeuralNet::crossover(NeuralNet mother, vector<unsigned int> parentShape)
{
   NeuralNet father = *this;
   vector<vector<vector<double> > > motherWeights = mother.getWeights();
   //vector<vector<vector<double> > > fatherWeights = father.getWeights();
   unsigned int i, j, k;
   NeuralNet child = father;
   for(i = 0; i < motherWeights.size(); i++)
   {
      for(j = 0; j < motherWeights.at(i).size(); j++)
      {
         for (k = 0; k < motherWeights.at(i).at(j).size(); k++)
         {
            if(coinFlip() == 0)
            {
               child.weights.at(i).at(j).at(k) = motherWeights.at(i).at(j).at(k);
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
            child.weights.at(i).at(j).at(k) = mutation(child.weights.at(i).at(j).at(k));  
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
   //int numWeights = 0;
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
   //cout << "Weights saved: " <<  numWeights << endl;
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
   //gene = normalShake(gene);
   //return randomMutation(gene);
   return normalShake(gene);
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

double NeuralNet::normalShake(double gene)
{
   double adjust;
   normal_distribution<double> distribution(0.0, 1.0);
   adjust = distribution(generator);
   return gene + adjust;
}

double NeuralNet::uniformShake(double gene)
{
   double adjust = randomDouble();
   return gene + adjust;
}

double NeuralNet::cauchyShake(double gene)
{
   cauchy_distribution<double> cDistribution(0.0,1.0);
   double adjust = cDistribution(generator);
   return gene + adjust;
}
//new
double NeuralNet::softplus(double x)
{
   double arg = 1 + exp(x);
   if(x < log(DBL_MAX)) 
   {
      if(arg >=0)
      {
         x = log(arg);
         return x;
      }
      else
      {
         cout << "Negative input to log inside softplus " << endl;
      }
   }
   return x;
   
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
    double denom;
   denom = 1 + abs(x);
   if(denom != 0)
   {
      return x / denom;
   }
   else 
   {
      cout << "divide by zero in softsign" << endl;
   }
   /*if(denom < 0.1)
   {
      cout << "Small Denominator: " << denom << endl;
   }*/
   return x;
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
   double eToTheX = exp(-x);
   double denom = eToTheX + 1;
   double result;
   if(denom != 0)
   {
      result = eToTheX / denom;
      //cout << x << endl; 
      if(isnan(x))
      {
         cout << "input = " << x << endl;
      }
      return result;
   }
   else 
   { 
      cout << "Div by zero in sigmoid" << endl;
      exit(3);
   }
   
   return x;
}
