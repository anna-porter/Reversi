#include "reversi.h"
#include "neuralNet.h"
#include "evolution.h"



int reversiAgentOneMove(Board, player, Organism);

Organism::Organism(vector<unsigned int> layerSizes)
{
   //cout << "layersizes size inside organism " << layerSizes.size() << endl;
   brain = NeuralNet(layerSizes);
   //cout << "made Net " << endl;
   fitness = 0;
   this->layerSizes = layerSizes;
}

Population::Population(vector<unsigned int> layerSizes, int populationSize)
{
   //cout << "layersizes size inside population" << layerSizes.size() << endl;
   Organism currentGuy = Organism(layerSizes);
   // Change to two for loops
   int i, j;
   vector<Organism> row;
   popSize = populationSize;
   for(i = 0; i < popSize; i++)
   {
      for(j = 0; j < popSize; j++)
      {
         currentGuy = Organism(layerSizes);
         row.push_back(currentGuy);
      }
      pop.push_back(row);
      row.clear();
   }
}

void Population::playNeighbors()
{
   int i, j, rowInc, colInc, row, column;
   int size;
   size = this->getSize();
   //cout << size << endl;
   pair<int, int> fitnessUpdate;
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         for(rowInc = -1; rowInc <= 1; rowInc++)
         {
            for(colInc = -1; colInc <= 1; colInc++)
            {
               if(rowInc != colInc)
               {
                  row = (i + size + rowInc) % size;
                  column = (j + size + colInc) % size;
                  pop.at(i).at(j);
                  pop.at(row).at(column);
                  fitnessUpdate = playGame(pop.at(i).at(j), pop.at(row).at(column));
                  pop.at(i).at(j).updateFitness(fitnessUpdate.first);
                  pop.at(row).at(column).updateFitness(fitnessUpdate.second);
                  cout << "organism " << i << " , " << j << " vs. organism " << row << " , " << column << endl; 
                  cout << fitnessUpdate.first << " , " << fitnessUpdate.second << endl;
               }
            }
         }
         cout << endl << endl;
      }
   }
}

pair<int, int> Population::playGame(Organism blackPlayer, Organism whitePlayer)
{
   srandom(time(0));
   // white and then black for this pair
   pair<int, int> fitnesses;
   int emptyPieces;
   fitnesses.first = 0;
   fitnesses.second = 0;
   int size;
   Board reversiBoard = Board(8);
   size = reversiBoard.getSize();
    while(!reversiBoard.isGameOver())
   {
      
      // player A goes first, if they still have valid moves,
      // Otherwise, pass to black
      if(reversiBoard.getValidMoves(Black).size() != 0)
      {
         int playerBlack_move = reversiAgentOneMove(reversiBoard, Black, blackPlayer);
         
         //if it is an invalid move, B automatically wins
         if(!reversiBoard.isValidMove(playerBlack_move, Black))
         {
            cout << playerBlack_move << " : " << "Black made an invalid move" << endl;
            //reversiBoard.printBoard();
         }
         // Update the board for player A
         else
         {
            //cout << "make black's move" << endl;
            reversiBoard.makeMove(playerBlack_move, Black);
            //reversiBoard.printBoard();
         }

         // if playerA made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            // White is first in this pair.
            cout << "White: " << finalScore.first << endl;
            cout << "Black: " << finalScore.second << endl;
            // WINNER TAKES EMPTY SPACES
            // if black wins
            if(finalScore.first > finalScore.second)
            {
               // First Organism gets 64 points for winning,
               fitnesses.first += size * size;
               // see how many empty pieces r on the board
               emptyPieces = size * size - (finalScore.first + finalScore.second);
               // winner takes credit for empty spaces.
               fitnesses.first = fitnesses.first + emptyPieces + finalScore.first;
               // loser only gets the number of his tiles
               fitnesses.second += finalScore.second;
            }
            // if white wins
            else if (finalScore.first < finalScore.second)
            {
               fitnesses.second += size * size;
               emptyPieces = size * size - (finalScore.second + finalScore.first);
               fitnesses.second = fitnesses.second + emptyPieces + finalScore.second;
               fitnesses.first += finalScore.first;
            }
            // if there is a tie
            else
            {
               fitnesses.first += (size * size) / 2 + finalScore.first;
               fitnesses.second += (size * size) / 2 + finalScore.second;
            }
         }
      }
      else 
      {
         //cout << endl << "Black has no valid moves and passes to White"  << endl;
      }
      if(reversiBoard.getValidMoves(White).size() != 0)
      {
         int playerWhite_move = reversiAgentOneMove(reversiBoard, White, whitePlayer);
         // if it is an invalid move, A automatically wins
         if(!reversiBoard.isValidMove(playerWhite_move, White))
         {
            //cout << playerWhite_move << " : "<<"White made an invalid move" << endl;
            //cout << playerWhite_move << " : "<<"White made an invalid move" << endl;
            //reversiBoard.printBoard();
         }
         // Update the board for player
         else
         {
            reversiBoard.makeMove(playerWhite_move, White);
            //reversiBoard.printBoard();
         }

         // if playerBlack made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            cout << "Black: " << finalScore.first << endl;
            cout << "White: " << finalScore.second << endl;
            // WINNER TAKES EMPTY SPACES
            // if black wins
            if(finalScore.first > finalScore.second)
            {
               // First Organism gets 64 points for winning,
               fitnesses.first += size * size;
               // see how many empty pieces r on the board
               emptyPieces = size * size - (finalScore.first + finalScore.second);
               // winner takes credit for empty spaces.
               fitnesses.first = fitnesses.first + emptyPieces + finalScore.first;
               // loser only gets the number of his tiles
               fitnesses.second += finalScore.second;
            }
            // if white wins
            else if (finalScore.first < finalScore.second)
            {
               fitnesses.second += size * size;
               emptyPieces = size * size - (finalScore.second + finalScore.first);
               fitnesses.second = fitnesses.second + emptyPieces + finalScore.second;
               fitnesses.first += finalScore.first;
            }
            // if there is a tie
            else
            {
               fitnesses.first += size * size;
               fitnesses.second += size * size;
            }
         }
      }
      else 
      {
         //cout << endl << "White has no moves, and passes to Black" << endl;
      }

   }

   
   return fitnesses;
}

vector<vector<int> > Population::getAllFitnesses()
{
   vector<int> row;
   vector<vector<int> > fitnessVec;
   int size = this->getSize();
   int i,j;
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         row.push_back(pop.at(i).at(j).getFitness());
      }
      fitnessVec.push_back(row);
      row.clear();
   }
   return fitnessVec;
}

int reversiAgentOneMove(Board reversiBoard, player mover, Organism org)
{
   vector<double> inputs;
   Board copyBoard;
   int bestMove;
   unsigned int i;
   double maxHeuristic, heuristic;
   maxHeuristic = -1000;
   bestMove = -1;
   heuristic = 0;
   NeuralNet network = org.getNet();
   vector<int> availableMoves;
   availableMoves = reversiBoard.getValidMoves(mover);
   //cout << "num of avail moves : "  << availableMoves.size() << endl;
   for(i = 0; i < availableMoves.size(); i++)
   {
      
      // only evaluating one move ahead so it should be reset each time.
      copyBoard = reversiBoard;
      if(copyBoard.isValidMove(availableMoves.at(i), mover))
      {
         copyBoard.makeMove(availableMoves.at(i), mover);
         inputs = copyBoard.boardToInput(mover);
         heuristic = network.calculateNet(inputs);
         if(heuristic > maxHeuristic)
         {
            maxHeuristic = heuristic;
           
            bestMove = availableMoves.at(i);
         }
      }
   }
   if(bestMove != -1)
   {
      return bestMove;
   }
   return bestMove; // return something else maybe?
}

Population Population::createNextGen()
{
   int i, j, k, rowInc, colInc, row, column;
   int size;
   int currentFit, lowestFit;
   srandom(time(0));
   lowestFit = 999999;
   NeuralNet mother, father, child;
   size = this->getSize();
   Population oldGen = *this;
   vector<unsigned int> layers = oldGen.pop.at(0).at(0).getLayerSizes();
   Population nextGen(layers, size);
   Organism parentOrg(layers);
   vector<Organism> parentCandidates;
   vector<int> fitnessProb;
   int randomNum, total;
   total = 0;
   vector<int> parentRow;
   vector<int> parentColumn;
   //pair<int, int> fitnessUpdate;
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         for(rowInc = -1; rowInc <= 1; rowInc++)
         {
            for(colInc = -1; colInc <= 1; colInc++)
            {
               if(rowInc != colInc)
               {
                  
                  row = (i + size + rowInc) % size;
                  column = (j + size + colInc) % size;
                  currentFit = oldGen.pop.at(row).at(column).getFitness();
                  //cout << currentFit << " , ";
                  if(currentFit < lowestFit)
                  {
                     lowestFit = currentFit;
                  }
                  parentCandidates.push_back(oldGen.pop.at(row).at(column));
                  parentRow.push_back(row);
                  parentColumn.push_back(column);
               }
            }
         }
         //cout << endl;
         //cout << "lowestFit is " << lowestFit << endl;
         
         for(rowInc = -1; rowInc <= 1; rowInc++)
         {
            for(colInc = -1; colInc <= 1; colInc++)
            {
               if(rowInc != colInc)
               {
                  row = (i + size + rowInc) % size;
                  column = (j + size + colInc) % size;
                  currentFit = oldGen.pop.at(row).at(column).getFitness();
                  //cout << currentFit << " - " << lowestFit << " = " << currentFit - lowestFit << endl;
                  fitnessProb.push_back(currentFit - lowestFit);
                  total += currentFit - lowestFit;
               }
            }
         }
         // testing'
         //cout << endl << endl;
         //cout << "fitness Probabilities: " << endl;
         //for(k = 0; k < fitnessProb.size(); k++)
         //{
         //   cout << fitnessProb.at(k) << endl;
         //}
         // end testing
         // for each of the six neighbors
         int condition;
         condition = 0;
         bool parentFound = false;
         //cout << "total: " << total << endl;
         randomNum = randomInt(total - 1);
         //cout << "RandomNum : " << randomNum << endl;
         //cout << "conditions: " << endl;
         for(k = 0; k < fitnessProb.size(); k++)
         {
            condition += fitnessProb.at(k);
            //cout << condition << endl;
            if(randomNum <= condition && !parentFound)
            {
               parentFound = true;
               cout << "Organism " <<i << j<< "picked parent " << parentRow.at(k) << parentColumn.at(k) << endl;
               parentOrg = parentCandidates.at(k);
            }
         }
         total = 0;
         lowestFit = 99999;
         fitnessProb.clear();
         parentRow.clear();
         parentColumn.clear();
         parentCandidates.clear();
         //cout << i << " : " << j << endl;
         mother = oldGen.pop.at(i).at(j).getNet();
         father = parentOrg.getNet();
         child = father.crossover(mother, layers);
         nextGen.pop.at(i).at(j).setNet(child);
      }
   }
   return nextGen;
}

void Population::assignFittnesses()
{
   int i, j;
   int size, newFitness;
   size = this->getSize();
   srandom(time(0));
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         newFitness = randomInt(768);
         pop.at(i).at(j).updateFitness(newFitness);
      }
   }
}
/*void Population::recreatePopulation();
{
   
}
void Population::ouputPopulation()
{
   
}*/
/*
Population Population::runGenerations(int generations, vector<unsigned int> layerSizes, int size)
{
   int i;
   Population currentGen(layerSizes, size);
   // this initialization is just a place holdler;
   Population nextGen(layerSizes, size);
   for(i = 0; i < generations; i++)
   {
      currentGen.playNeighbors();
      currentGen.createNextGen();
   }
}*/
Population& Population::operator=(const Population &rhs)
{
   this->pop.clear();
   this->pop = rhs.getPop();
   return *this;
}
void Population::printPopulation()
{
   Population populus = *this;
   int size, i, j;
   size = populus.getSize();
   NeuralNet network;
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         cout << "Organism " << i << " , " << j << " : " << endl;
         network = populus.pop.at(i).at(j).getNet();
         network.printWeights();
      }
   }
}
int main()
{
   /*
   // Testing for crossover on a population with small neuralnets
   vector<unsigned int> layerSizes;
   layerSizes.push_back(3);
   layerSizes.push_back(2);
   layerSizes.push_back(1);
   Population populus = Population(layerSizes, 4);
   populus.assignFittnesses();
   Population newGeneration(layerSizes, 4);
   newGeneration = populus.createNextGen();
   cout << "Old population: " << endl;
   populus.printPopulation();
   cout << endl << endl << "New population: " << endl; 
   newGeneration.printPopulation();
   */
   
   //populus.playNeighbors();
   //cout << "after playing their neighbors" << endl;
   //Population nextGeneration = Population(layerSizes, 4);
   
   /*int size, i, j;
   size = populus.getSize();
   //cout << "size: " << size << endl;
   vector<vector<int> > fitnessVec;
   fitnessVec = populus.getAllFitnesses();
   cout << "after getting all fitnesses" << endl;
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         cout << fitnessVec.at(i).at(j) << " + ";
      }
      cout << endl;
   }*/

   return 0;
}
