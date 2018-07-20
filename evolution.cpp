#include "reversi.h"
#include "neuralNet.h"
#include "evolution.h"
#include <sstream>
int reversiAgentOneMove(Board, player, Organism);
Organism::Organism(vector<unsigned int> layerSizes)
{
   brain = NeuralNet(layerSizes);
   fitness = 0;
   this->layerSizes = layerSizes;
}

Organism::Organism(vector<vector<vector<double> > > netweights)
{
   brain = NeuralNet(netweights);
}

Organism::Organism()
{
   fitness = 0;
   layerSizes.clear();
}

Population::Population(vector<vector<Organism> > newPop)
{
   pop = newPop;
}

Population::Population(vector<unsigned int> layerSizes, int popSize)
{
   Organism currentGuy = Organism(layerSizes);
   int i, j;
   vector<Organism> row;
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

Population::Population()
{
   pop.clear();
}

void Population::playNeighbors()
{
   int i, j, rowInc, colInc, row, column;
   int size;
   size = this->pop.size();
   cout << "this.size() = " << this->pop.size() << endl;
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
                  //cout << "organism " << i << " , " << j << " vs. organism " << row << " , " << column << endl; 
                  //cout << fitnessUpdate.first << " , " << fitnessUpdate.second << endl;
               }
            }
         }
         //cout << endl << endl;
      }
   }
}

pair<int, int> Population::playGame(Organism blackPlayer, Organism whitePlayer)
{
   srandom(time(0));
   // black and then white for this pair
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
            //cout << "White: " << finalScore.first << endl;
            //cout << "Black: " << finalScore.second << endl;
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
            cout << playerWhite_move << " : "<<"White made an invalid move" << endl;
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
            //cout << "Black: " << finalScore.first << endl;
            //cout << "White: " << finalScore.second << endl;
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

pair<int, int> Population::playGamePrint(Organism blackPlayer, Organism whitePlayer)
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
            reversiBoard.printBoard();
         }
         // Update the board for player A
         else
         {
            //cout << "make black's move" << endl;
            reversiBoard.makeMove(playerBlack_move, Black);
            reversiBoard.printBoard();
         }

         // if playerA made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            // White is first in this pair.
            //cout << "White: " << finalScore.first << endl;
            //cout << "Black: " << finalScore.second << endl;
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
            cout << playerWhite_move << " : "<<"White made an invalid move" << endl;
            reversiBoard.printBoard();
         }
         // Update the board for player
         else
         {
            reversiBoard.makeMove(playerWhite_move, White);
            reversiBoard.printBoard();
         }

         // if playerBlack made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            //cout << "Black: " << finalScore.first << endl;
            //cout << "White: " << finalScore.second << endl;
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
   int size = this->pop.size();
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
   int i, j, rowInc, colInc, row, column;
   unsigned int k;
   int size;
   int currentFit, lowestFit;
   srandom(time(0));
   lowestFit = 999999;
   NeuralNet mother, father, child;
   size = this->pop.size();
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
            //cout << fitnessProb.at(k) << endl;
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
               cout << "Organism " <<i << j<< "picked parent " << k << " at " << parentRow.at(k) << parentColumn.at(k)  << endl;
               //cout << k << endl;
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

void Population::assignFitnesses()
{
   int i, j;
   int size, newFitness;
   size = this->pop.size();
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

Population Population::runGenerations(int generations, vector<unsigned int> layerSizes, int size)
{
   int i;
   Population currentGen = *this;
   Population nextGen(layerSizes, size);
   for(i = 0; i < generations; i++)
   {
      cout << "Generation " << i << endl;
      currentGen.playNeighbors();
      nextGen = currentGen.createNextGen();
      currentGen = nextGen;
      if(i % 10)
      {
         currentGen.savePopulation(i);
      }
   }
   return currentGen;
   // save after about 100 generations;
}

Population& Population::operator=(const Population &rhs)
{
   this->pop.clear();
   this->pop = rhs.getPop();
   return *this;
}

void Population::printPopulation()
{
   cout << "inside Print population" << endl;
   Population populus = *this;
   int size, i, j;
   size = populus.pop.size();
   NeuralNet network;
   cout << "size " << size << endl;
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

void Population::resetFitnesses()
{
   Population populus = *this;
   int i, j;
   int size = populus.pop.size();
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         populus.pop.at(i).at(j).setFitness(0);
      }
   }
}
string fileString(int genNum)
{
   stringstream strstr;
   strstr << "Gen_" << genNum << ".txt";
   return strstr.str();
}
void Population::savePopulation(int genNum)
{
   //https://stackoverflow.com/questions/13108973/creating-file-names-automatically-c
   Population populus = *this;
   //int num;
   //int digit;
   ofstream fout;
   //string fileName = "" + fileString(genNum);
   /*string fileName = "Gen_ ";
   while(genNum > 0)
   {
      digit = genNum % 10;
      genNum =  genNum / 10;
      switch (digit)
      {
         case 1: fileName += "1"; break;
         case 2: fileName += "2"; break;
         case 3: fileName += "3"; break;
         case 4: fileName += "4"; break;
         case 5: fileName += "5"; break;
         case 6: fileName += "6"; break;
         case 7: fileName += "7"; break;
         case 8: fileName += "8"; break;
         case 9: fileName += "9"; break;
         case 0: fileName += "0"; break;
         
      }
   }
   fileName += ".txt";*/
   fout.open("testing.txt");
   int i, j, size;
   size = populus.pop.size();
   NeuralNet network;
   fout << "size " << endl << size << endl;
   for(i = 0; i < size; i++)
   {
      for(j = 0;  j < size; j++)
      {
         network = populus.getOrganism(i, j).getNet();
         network.saveWeights(fout);
      }
   }
   fout.close();
}

// asked Michael for help on this function;
Population Population::loadPopulation()
{
   ifstream input;
   input.open("Gen_1.txt");
   string token, weightString, numString;
   int lastDig, size;
   size = 0; // to keep compuler from warning me
   double weight;
   Population newPop;
   Organism placeholder;
   vector<vector<Organism> > organismVecs;
   vector<Organism> tempOrganisms;
   vector<vector<vector<double> > > netWeights;
   vector<vector<double> > tempLayer;
   vector<double> tempNeuron;
   if(input.is_open())
   {
      getline(input, token);
      if(token == "size")
      {
         getline(input, token);
         size = atof(token.c_str());
      }
      while(getline(input, token))
      {
         if(token == "newNeuron")
         {
            tempNeuron.clear();
            getline(input, weightString);
            while(weightString.size() > 0)
            {
               lastDig = weightString.find(",");
               numString = weightString.substr(0, lastDig);
               weight = atof(numString.c_str());
               tempNeuron.push_back(weight);
               weightString.erase(0, lastDig + 1);
            }
            tempLayer.push_back(tempNeuron);
         }
         else if(token == "newLayer")
         {  
            if(tempLayer.size() == 0)
            {
               netWeights.clear();
               tempLayer.clear();
            }
            else
            {
               netWeights.push_back(tempLayer);
               tempLayer.clear();
            }
         }
         else if(token == "endOfOrganism")
         {
            netWeights.push_back(tempLayer);
            placeholder = Organism(netWeights);
            tempOrganisms.push_back(placeholder);
            netWeights.clear();
            tempLayer.clear();
         }
      }
   }
   vector<Organism> tempRow;
   for(int i = 0; i < size; i++)
   {
      for(int j = 0; j < size; j++)
      {
         tempRow.push_back(tempOrganisms.at(i * size + j));
      }
      organismVecs.push_back(tempRow);
      tempRow.clear();
   }  
   input.close();
   Population generatedPop = Population(organismVecs);
   return generatedPop;
}

void Population::popVSpop(Population teamB)
{
   Population teamA = *this;
   int sizeOfA, sizeOfB, rowA, colA, rowB, colB, i, j, temp;
   sizeOfA = teamA.pop.size();
   sizeOfB = teamB.pop.size();
   int winsAsWhiteA[sizeOfA * sizeOfA], winsAsWhiteB[sizeOfB * sizeOfB];
   int winsAsBlackA[sizeOfA * sizeOfA], winsAsBlackB[sizeOfB * sizeOfB];
   int lossesAsWhiteA[sizeOfA * sizeOfA], lossesAsWhiteB[sizeOfB * sizeOfB];
   int lossesAsBlackA[sizeOfA * sizeOfA], lossesAsBlackB[sizeOfB * sizeOfB];
   int tiesAsWhiteA[sizeOfA * sizeOfA], tiesAsWhiteB[sizeOfB * sizeOfB];
   int tiesAsBlackA[sizeOfA * sizeOfA], tiesAsBlackB[sizeOfB * sizeOfB];
   int piecesControlledA[sizeOfA * sizeOfA], piecesControlledB[sizeOfB * sizeOfB];
   int orderA[sizeOfA * sizeOfA], orderB[sizeOfB * sizeOfB];
   string agentStrA[sizeOfA * sizeOfA], agentStrB[sizeOfB * sizeOfB];
   int fitnessA[sizeOfA * sizeOfA], fitnessB[sizeOfB * sizeOfB];
   pair <int, int> fitnessUpdate;
   int gamesPlayed = 0;
   for(rowA = 0; rowA < sizeOfA; rowA++)
   {
      for(colA = 0; colA < sizeOfA; colA++)
      {
         winsAsWhiteA[rowA * sizeOfA + colA] = 0;
         winsAsBlackA[rowA * sizeOfA + colA] = 0;
         lossesAsWhiteA[rowA * sizeOfA + colA] = 0;
         lossesAsBlackA[rowA * sizeOfA + colA] = 0;
         tiesAsWhiteA[rowA * sizeOfA + colA] = 0;
         tiesAsBlackA[rowA * sizeOfA + colA] = 0;
         piecesControlledA[rowA * sizeOfA + colA] = 0;
         stringstream strstr;
         strstr << "Org: " << rowA << " , " << colA;
         agentStrA[rowA * sizeOfA + colA] = strstr.str();
         strstr.clear();
      }
   }
   for(rowB = 0; rowB < sizeOfB; rowB++)
   {
      for(colB = 0; colB < sizeOfB; colB++)
      {
         winsAsWhiteB[rowB * sizeOfB + colB] = 0;
         winsAsBlackB[rowB * sizeOfB + colB] = 0;
         lossesAsWhiteB[rowB * sizeOfB + colB] = 0;
         lossesAsBlackB[rowB * sizeOfB + colB] = 0;
         tiesAsWhiteB[rowB * sizeOfB + colB] = 0;
         tiesAsBlackB[rowB * sizeOfB + colB] = 0;
         piecesControlledB[rowB * sizeOfB + colB] = 0;
         stringstream strstr;
         strstr << "Org: " << rowB << " , " << colB;
         agentStrB[rowB * sizeOfB + colB] = strstr.str();
         strstr.clear();
      }
   }
   for(rowA = 0; rowA < sizeOfA; rowA++)
   {
      for(colA = 0; colA < sizeOfA; colA++)
      {
         for(rowB = 0; rowB < sizeOfB; rowB++)
         {
            for(colB = 0; colB < sizeOfB; colB++)
            {
               // black and then white
               fitnessUpdate = playGame(teamA.getOrganism(rowA, colA), teamB.getOrganism(rowB, colB));
               teamA.pop.at(rowA).at(colA).updateFitness(fitnessUpdate.first);
               teamB.pop.at(rowB).at(colB).updateFitness(fitnessUpdate.second);
               gamesPlayed++;
               if(fitnessUpdate.first > fitnessUpdate.second)
               {
                  winsAsBlackA[rowA * sizeOfA + colA] += 1;
                  lossesAsWhiteB[rowB * sizeOfB + colB] += 1;
               }
               else if (fitnessUpdate.second > fitnessUpdate.first)
               {
                  winsAsWhiteB[rowB * sizeOfB + colB] += 1;
                  lossesAsBlackA[rowA * sizeOfA + colA] += 1;
               }
               else
               {
                  tiesAsBlackA[rowA * sizeOfA + colA] += 1;
                  tiesAsWhiteB[rowB * sizeOfB + colB] += 1;
               }
               fitnessUpdate = playGame(teamB.getOrganism(rowB, colB), teamB.getOrganism(rowA, colA));
               teamB.pop.at(rowB).at(colB).updateFitness(fitnessUpdate.first);
               teamA.pop.at(rowA).at(colA).updateFitness(fitnessUpdate.second);
               gamesPlayed++;
               if(fitnessUpdate.first > fitnessUpdate.second)
               {
                  winsAsBlackB[rowB * sizeOfB + colB] += 1;
                  lossesAsWhiteA[rowA * sizeOfA + colA] += 1;
               }
               else if (fitnessUpdate.second > fitnessUpdate.first)
               {
                  winsAsWhiteA[rowA * sizeOfA + colA] += 1;
                  lossesAsBlackB[rowB * sizeOfB + colB] += 1;
               }
               else
               {
                  tiesAsBlackB[rowB * sizeOfB + colB] += 1;
                  tiesAsWhiteA[rowA * sizeOfA + colA] += 1;
               }
            }
         }
      }
   }
   int totalFit;
   int pieces;
   // CHANGE IF WE CHANGE TO 6X6
   for(rowA = 0; rowA < sizeOfA; rowA++)
   {
      for(colA = 0; colA < sizeOfA; colA++)
      {
         totalFit = teamA.pop.at(rowA).at(colA).getFitness();
         fitnessA[rowA * sizeOfA + colA] = totalFit;
         pieces = totalFit - (winsAsBlackA[rowA * sizeOfA + colA] + winsAsWhiteA[rowA * sizeOfA + colA]) * 64 - (tiesAsBlackA[rowA * sizeOfA + colA] + tiesAsWhiteA[rowA * sizeOfA + colA]) * 32;
         piecesControlledA[rowA * sizeOfA + colA] = pieces;
      }
   }
   for(rowB = 0; rowB < sizeOfB; rowB++)
   {
      for(colB = 0; colB < sizeOfB; colB++)
      {
         totalFit = teamB.pop.at(rowB).at(colB).getFitness();
         fitnessB[rowB * sizeOfB + colB] = totalFit;
         pieces = totalFit - (winsAsBlackB[rowB * sizeOfB + colB] + winsAsWhiteB[rowB * sizeOfB + colB]) * 64 - (tiesAsBlackB[rowB * sizeOfB + colB] + tiesAsWhiteB[rowB * sizeOfB + colB]) * 32;
         piecesControlledB[rowB * sizeOfB + colB] = pieces;
      }
   }
   for (i = 0; i < sizeOfA * sizeOfA; i += 1)
   {
      orderA[i] = i;
   }
   for(i = 0; i < sizeOfB * sizeOfB; i+= 1)
   {
      orderB[i] = i;
   }
   for (i = 0; i < sizeOfA * sizeOfA - 1; i += 1)
   {
      for (j = i + 1; j < sizeOfA * sizeOfA; j += 1)
      {
      if (winsAsWhiteA[orderA[i]] + winsAsBlackA[orderA[i]] < winsAsWhiteA[orderA[j]] + winsAsBlackA[orderA[j]] || winsAsWhiteA[orderA[i]] + winsAsBlackA[orderA[i]] == winsAsWhiteA[orderA[j]] + winsAsBlackA[orderA[j]] && piecesControlledA[orderA[i]] > piecesControlledA[orderA[j]] || piecesControlledA[orderA[i]] == piecesControlledA[orderA[j]]  && agentStrA[orderB[i]] >= agentStrA[orderB[j]]) // sort the agents from best to wors
         {
            temp = orderA[i];
            orderA[i] = orderA[j];
            orderA[j] = temp;
         }
      }
   }
   for (i = 0; i < sizeOfB * sizeOfB - 1; i += 1)
   {
      for (j = i + 1; j < sizeOfB * sizeOfB; j += 1)
      {
         if (winsAsWhiteB[orderB[i]] + winsAsBlackB[orderB[i]] < winsAsWhiteB[orderB[j]] + winsAsBlackB[orderB[j]] || winsAsWhiteB[orderB[i]] + winsAsBlackB[orderB[i]] == winsAsWhiteB[orderB[j]] + winsAsBlackB[orderB[j]] && (piecesControlledB[orderB[i]] > piecesControlledB[orderB[j]] || piecesControlledB[orderB[i]] == piecesControlledB[orderB[j]]  && agentStrB[orderB[i]] >= agentStrB[orderB[j]])) // sort the agents from best to worst
         {
            temp = orderB[i];
            orderB[i] = orderB[j];
            orderB[j] = temp;
         }
      }
   }
   cout << "\n" << "Team A: " << "\n"
        << "Overall standings:       all             as Black          as White      Total #     W / L     Fitness\n"
        << "                      W    L    T       W    L    T       W    L    T    pieces      Ratio      Score\n";
   for (i = 0; i < sizeOfA * sizeOfA; i += 1)
   {
      double ratio;
      if (lossesAsBlackA[orderA[i]] + lossesAsWhiteA[orderA[i]] == 0) 
      {
         ratio = 1;
      }
      else 
      {
         ratio = (static_cast<double>(winsAsWhiteA[orderA[i]]) + static_cast<double>(winsAsBlackA[orderA[i]]))/(static_cast<double>((lossesAsWhiteA[orderA[i]]) + static_cast<double>(lossesAsBlackA[orderA[i]])));
      }
      cout << setw(18) << left << agentStrA[orderA[i]]
           << " " << setw(4) << right << winsAsWhiteA[orderA[i]] + winsAsBlackA[orderA[i]]
           << " " << setw(4) << right << lossesAsWhiteA[orderA[i]] + lossesAsBlackA[orderA[i]]
           << " " << setw(4) << right << tiesAsWhiteA[orderA[i]] + tiesAsBlackA[orderA[i]]
           << " " << setw(7) << right << winsAsBlackA[orderA[i]]
           << " " << setw(4) << right << lossesAsBlackA[orderA[i]]
           << " " << setw(4) << right << tiesAsBlackA[orderA[i]]
           << " " << setw(7) << right << winsAsWhiteA[orderA[i]]
           << " " << setw(4) << right << lossesAsWhiteA[orderA[i]]
           << " " << setw(4) << right << tiesAsWhiteA[orderA[i]]
           << " " << setw(8) << right << piecesControlledA[orderA[i]]
           //<< " " << setw(8) << right << numInvalid[orderA[i]] 
           << " " << setprecision(5) << setw(11) << right << ratio 
           << " " << setw(9) << right << fitnessA[orderA[i]] << "\n";
   }
   cout << "\n" << "Team B: " << "\n"
        << "Overall standings:       all             as Black          as White      Total #     W / L     Fitness\n"
        << "                      W    L    T       W    L    T       W    L    T    pieces      Ratio      Score\n";
  
   for (i = 0; i < sizeOfB * sizeOfB; i += 1)
   {
      double ratio; 
      if(lossesAsBlackB[orderB[i]] + lossesAsWhiteB[orderB[i]] == 0) 
      {
         ratio = 1;
      }
      else 
      {
         ratio = (static_cast<double>(winsAsWhiteB[orderB[i]]) + static_cast<double>(winsAsBlackB[orderB[i]]))/(static_cast<double>((lossesAsWhiteB[orderB[i]]) + static_cast<double>(lossesAsBlackB[orderB[i]])));
      }
      cout << setw(18) << left << agentStrB[orderB[i]]
           << " " << setw(4) << right << winsAsWhiteB[orderB[i]] + winsAsBlackB[orderB[i]]
           << " " << setw(4) << right << lossesAsWhiteB[orderB[i]] + lossesAsBlackB[orderB[i]]
           << " " << setw(4) << right << tiesAsWhiteB[orderB[i]] + tiesAsBlackB[orderB[i]]
           << " " << setw(7) << right << winsAsBlackB[orderB[i]]
           << " " << setw(4) << right << lossesAsBlackB[orderB[i]]
           << " " << setw(4) << right << tiesAsBlackB[orderB[i]]
           << " " << setw(7) << right << winsAsWhiteB[orderB[i]]
           << " " << setw(4) << right << lossesAsWhiteB[orderB[i]]
           << " " << setw(4) << right << tiesAsWhiteB[orderB[i]]
           << " " << setw(8) << right << piecesControlledB[orderB[i]]
           //<< " " << setw(8) << right << numInvalid[orderA[i]] 
           << " " << setprecision(5) << setw(11) << right << ratio 
           << " " << setw(9) << right << fitnessB[orderB[i]] << "\n";
   }
}
void Population::printFitnesses()
{
   Population populus = *this;
   int size, i, j;
   size = populus.pop.size();
   vector<vector<int> > fitnessVec;
   fitnessVec = populus.getAllFitnesses();
   for(i = 0; i < size; i++)
   {
      for(j = 0; j < size; j++)
      {
         cout << fitnessVec.at(i).at(j) << " + ";
      }
      cout << endl;
   }
}
int main()
{
   vector<unsigned int> layerSizes;
   srandom(time(0));
   
   // Testing for crossover on a population with small neuralnets
   //vector<unsigned int> layerSizes;
   //layerSizes.push_back(3);
   /*layerSizes.push_back(2);
   layerSizes.push_back(1);
   Population populus = Population(layerSizes, 3);
   populus.assignFitnesses();
   Population newGeneration(layerSizes, 3);
   newGeneration = populus.createNextGen();
   cout << "Old population: " << endl;
   populus.printPopulation();
   cout << endl << endl << "New population: " << endl; 
   newGeneration.printPopulation();*/
   //populus.playNeighbors(); 
   //cout << "after playing their neighbors" << endl;
   //Population nextGeneration = Population(layerSizes, 4);
   /*
   int size, i, j;
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

   // TESTING A BASIC LOAD SAVE POPULATION:
   /*layerSizes.push_back(3);
   layerSizes.push_back(2);
   layerSizes.push_back(1);
   int sizePop = 3; 
   Population populus = Population(layerSizes, sizePop);
   populus.savePopulation(1);
   Population newPopulus;
   newPopulus = newPopulus.loadPopulation();
   cout << "old population: " << endl;
   populus.printPopulation();
   cout << endl << endl << "Loaded Population" << endl;
   newPopulus.printPopulation();
   */
   
   layerSizes.push_back(64);
   layerSizes.push_back(8);
   layerSizes.push_back(1);
   Population popA = Population(layerSizes, 3);
   Population popB = Population(layerSizes, 4);
   popA.popVSpop(popB);
   /*
   cout << "fitnesses of A" << endl;
   popA.printFitnesses();
   cout << "fitnesses of B" << endl;
   popB.printFitnesses();
   popA.resetFitnesses();
   popB.resetFitnesses();
   popA.printFitnesses();
   popB.printFitnesses();
   */
   //NEED TO CLEAR FITNESSES AFTER POPVSPOP;
   //using argc and argv to take in a name of a file to load
   
   /*Organism black1gen = populus.getOrganism(4, 4);
   Population pop10thGen = populus.runGenerations(10, layerSizes, 10);
   Organism white10gen = pop10thGen.getOrganism(4,4);
   pair<int, int> scores,scores2;
   scores = populus.playGamePrint(black1gen, white10gen);
   //scores2 = pop10thGen.playGame(black1gen, white10gen); 
   cout << "Black 1st gen: " << scores.first << endl;
   cout << "White 10th gen: " << scores.second << endl; 
   */
   /*
   Population pop20thGen = pop10thGen.runGenerations(10, layerSizes, 10);
   Organism white20Gen = pop20thGen.getOrganism(4,4);
   scores = populus.playGamePrint(black1gen, white20Gen);
   cout << "Black 1st gen: " << scores.first << endl;
   cout << "white 20th gen: " << scores.second << endl;
   */
   return 0;
   
}
