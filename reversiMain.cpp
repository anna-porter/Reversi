// CS Research, Summer 2
// Reversi Game

// Here's the main function that plays all agents against each other and
// summarizes the results.

// Real main based off of ORR and CRR

#include "reversi.h"
 
extern const int numAgents;
extern int (*agentFunc[])(const Board &, player);
extern string agentStr[];

//int reversiAgentRandom(const Board &board, player whichPlayer);
//int reversiAgentRandomClone(const Board &board, player whichPlayer);


char playReversiGame(int (*agentWhite)(const Board&, player whichPlayer), int (*agentBlack)(const Board&, player whichPlayer), Board& reversiBoard);

int main()
{
   char result;
   int  i, j, boardSize, numWinsWhite[numAgents], numWinsBlack[numAgents],
       numLossesWhite[numAgents], numLossesBlack[numAgents], /*numMoves[numAgents],*/ order[numAgents], numInvalid[numAgents], temp;

   Board *reversiBoard;

   //BOARD SIZE DECLARATION HERE
   boardSize = 8;
   
   cout << "Research Project: Reversi\n"
        << "Results\n\n";

   // intializes the arrays that will be used to sort for the top player
   for (i = 0; i < numAgents; i += 1)
   {
      numWinsWhite[i] = 0;
      numWinsBlack[i] = 0;
      numLossesWhite[i] = 0;
      numLossesBlack[i] = 0;
      //numMoves[i] = 0;
      // added initialization
      numInvalid[i] = 0;
   }

   // plays the game with all of the available agents and prints the board once like orr
      // needs to be messed with
   for (i = 0; i < numAgents; i += 1)
   {
      for (j = 0; j < numAgents; j += 1)
      {
         if (i != j)
         {
            //cout << "Current board size is " << boardSize << "x" << boardSize << "\n";
            reversiBoard = new Board(boardSize);
            cout << "player A = " << agentStr[i] << ", player B = " << agentStr[j];
            result = playReversiGame(agentFunc[i], agentFunc[j], *reversiBoard);
            //numMoves[i] += reversiBoard->getTurn();
            //numMoves[j] += reversiBoard->getTurn();
            //cout << "\nFinal Board State: \n";
            //reversiBoard->printBoard(); // will it update based on who played on it?
            cout << "\n";
            if (result == 'A')
            {
               cout << "A";
               numWinsWhite[i] += 1;
               numLossesBlack[j] += 1;
            }
            else if (result == 'B')
            {
               cout << "B";
               numLossesWhite[i] += 1;
               numWinsBlack[j] += 1;
            }
            else if (result == 'C')
            {
               cout << "B played an invalid move! A";
               numWinsWhite[i] += 1;
               numLossesBlack[j] +=1;
               numInvalid[j] += 1;
            }
            else
            {
               cout << "A played an invalid move! B";
               numLossesWhite[i] += 1;
               numWinsBlack[j] += 1;
               numInvalid[i] += 1;
            }
            cout << " is the winner!\n\n";
            delete reversiBoard;
         }
      }
   }

//      gameCount -= 1;
//      boardSize += 1;

   /*
      sort the agents based on total number of wins, then games as A, games as B and then the
      total number of moves made
   */
   for (i = 0; i < numAgents; i += 1)
   {
      order[i] = i;
   }

   for (i = 0; i < numAgents - 1; i += 1)
   {
      for (j = i + 1; j < numAgents; j += 1)
      {
         if (numWinsWhite[order[i]] + numWinsBlack[order[i]] < numWinsWhite[order[j]] + numWinsBlack[order[j]] || numWinsWhite[order[i]] + numWinsBlack[order[i]] == numWinsWhite[order[j]] + numWinsBlack[order[j]] /*&&
             (numMoves[order[i]] > numMoves[order[j]] || numMoves[order[i]] == numMoves[order[j]]  &&
              agentStr[order[i]] >= agentStr[order[j]]))*/) // sort the agents from best to worst
         {
            temp = order[i];
            order[i] = order[j];
            order[j] = temp;
         }
      }
   }

   // output the overall results of the game
      cout << "\n"
        << "Overall standings:      all        as White     as Black    Invalid     W / L\n"
        << "                       W    L       W    L       W    L      moves      Ratio\n";
   for (i = 0; i < numAgents; i += 1)
   {
      double ratio;
      if (numLossesWhite[order[i]] + numLossesBlack[order[i]] == 0) 
      {
         ratio = 1;
      }
      else 
      {
         ratio = (static_cast<double>(numWinsWhite[order[i]]) + static_cast<double>(numWinsBlack[order[i]]))/(static_cast<double>(numLossesWhite[order[i]]) + static_cast<double>(numLossesBlack[order[i]]));
      }
      cout << setw(20) << left << agentStr[order[i]]
           << " " << setw(4) << right << numWinsWhite[order[i]] + numWinsBlack[order[i]]
           << " " << setw(4) << right << numLossesWhite[order[i]] + numLossesBlack[order[i]]
           << " " << setw(7) << right << numWinsWhite[order[i]]
           << " " << setw(4) << right << numLossesWhite[order[i]]
           << " " << setw(7) << right << numWinsBlack[order[i]]
           << " " << setw(4) << right << numLossesBlack[order[i]]
           //<< " " << setw(8) << right << numMoves[order[i]]
           << " " << setw(8) << right << numInvalid[order[i]] 
           << " " << setprecision(5) << setw(11) << right << ratio << "\n";
   }

   return 0;
}

// needs to be written
char playReversiGame(int (*agentWhite)(const Board&, player whichPlayer), int (*agentBlack)(const Board&, player whichPlayer), Board& reversiBoard)
{
   char result = 'A'; // compiler complains without an initialization

   // repeatedly call playerA's and playerBlack's function until someone wins or makes an invalid move
   while(!reversiBoard.isGameOver())
   {
      // player A goes first, if they still have valid moves,
      // Otherwise, pass to black
      if(reversiBoard.getValidMoves(White).size() != 0)
      {
         int playerWhite_move = (*agentWhite)(reversiBoard, White);
         
         //if it is an invalid move, B automatically wins
         if(!reversiBoard.isValidMove(playerWhite_move, White))
         {
            // Let's let 'D' represent that player B won because of A's invalid move
            result = 'D';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player A
         else
         {
            reversiBoard.makeMove(playerWhite_move, White);
            reversiBoard.printBoard();
         }

         // if playerA made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            if(finalScore.first > finalScore.second)
            {
               result = 'A';
            }
            else if (finalScore.second > finalScore.first)
            {
               result = 'B';
            }

            return result;
         }
      }
      if(reversiBoard.getValidMoves(Black).size() != 0)
      {
         int playerBlack_move = (*agentBlack)(reversiBoard, Black);

         // if it is an invalid move, A automatically wins
         if(!reversiBoard.isValidMove(playerBlack_move, Black))
         {
            // Let's let 'C' represent that A won because of B's invalid move
            result = 'C';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player B
         else
         {
            reversiBoard.makeMove(playerBlack_move, Black);
            reversiBoard.printBoard();
         }

         // if playerBlack made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            if(finalScore.first > finalScore.second)
            {
               result = 'A';
            }
            else if (finalScore.second > finalScore.first)
            {
               result = 'B';
            }

            return result;
         }
      }

   }

   return result;
}
