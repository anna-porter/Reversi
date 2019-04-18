#include "reversi.h"
#include "neuralNet.h"
#include "evolution.h"

using namespace std;

int main(int argc, char* argv[])
{
   Population pops[argc - 1];
   int i, j;
   for(i = 1; i < argc; i++)
   {
      string fileName = argv[i - 1];
      pops[i - 1] = pops[i - 1].loadPopulation(fileName);
   }
   //string output;
   ofstream fout;
   fout.open("1Neuron10kThreshoildShort.txt");
   for(i = 0; i < argc - 1; i++)
   {
      for(j = 1; j < argc; j++)
      {
         if(j > i)
         {
            cout << argv[i] << " vs " << argv[j] << endl;
            fout << argv[i] << " vs " << argv[j] << endl;
            fout << pops[i].popVSpop(pops[j]);
         }
      }
   }
}
