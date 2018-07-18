// CS Research, Summer 2
// Reversi Game

// Here's the main function that plays all agents against each other and
// summarizes the results.

// Real main based off of ORR and CRR

#include "reversi.h"
#include "neuralNet.h"

extern const int numAgents;
extern int (*agentFunc[])(const Board &, player);
extern string agentStr[];
int reversiAgentRandom(const Board &board, player whichPlayer);
int randomInt(int n);

char playReversiGame(Board& reversiBoard);
char playReversiGameMidway(Board& reversiBoard);
/*
int main()
{
   char result;
   int boardSize;
   srandom(time(0));
   Board *reversiBoard;

   //BOARD SIZE DECLARATION HERE
   boardSize = 8;
   
   cout << "Research Project: Reversi\n"
        << "Results\n\n";
         {
            reversiBoard = new Board(boardSize);
            result = playReversiGame(*reversiBoard);
            
            // TESTING FOR BOARD TO INPUT HERE
            result = playReversiGameMidway(*reversiBoard);
            vector<double> whiteInputs, blackInputs;
            unsigned int i;
            whiteInputs = reversiBoard->boardToInput(White);
            blackInputs = reversiBoard->boardToInput(Black);
            cout << "White inputs are: " << endl;
            for(i = 0; i < whiteInputs.size(); i++)
            {
               if(i % 8 == 0)
               {
                  cout << endl;
               }
               cout << whiteInputs.at(i) << " , ";
            }
            cout << endl << endl;
            cout << "Black inputs are: " << endl;
            for(i = 0; i < blackInputs.size(); i++)
            {
               if(i % 8 == 0)
               {
                  cout << endl;
               }
               cout << blackInputs.at(i) << " , ";
            }
            cout << endl << endl;
            // END TESTING FOR BOARD TO INPUT
            
            cout << "\n";
            if (result == 'A')
            {
               cout << "White is the winner!\n\n";
            }
            else if (result == 'B')
            {
               cout << "Black is the winner!\n\n";
            }
            else if (result == 'C')
            {
               cout << "White played an invalid move! Black is the winner!\n\n";
            }
            else if (result == 'D')
            {
               cout << "Black played an invalid move! White is the winner!\n\n";
            }
            else 
            {
               cout << "The game resulted in a tie!" << endl << endl;
            }
            delete reversiBoard;
         }
   return 0;
}*/

// needs to be written
char playReversiGame(Board& reversiBoard)
{
   char result = 'A'; // compiler complains without an initialization

   // repeatedly call playerA's and playerBlack's function until someone wins or makes an invalid move
   while(!reversiBoard.isGameOver())
   {
      
      // player A goes first, if they still have valid moves,
      // Otherwise, pass to black
      if(reversiBoard.getValidMoves(Black).size() != 0)
      {
         int playerBlack_move = reversiAgentRandom(reversiBoard, Black);
         
         //if it is an invalid move, B automatically wins
         if(!reversiBoard.isValidMove(playerBlack_move, Black))
         {
            // Let's let 'D' represent that player B won because of A's invalid move
            result = 'D';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player A
         else
         {
            reversiBoard.makeMove(playerBlack_move, Black);
            cout << "Black: " << endl; 
            reversiBoard.printBoard();
         }

         // if playerA made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            // White is first in this pair.
            cout << "White: " << finalScore.second << endl;
            cout << "Black: " << finalScore.first << endl;
            if(finalScore.second > finalScore.first)
            {
               result = 'A';
            }
            else if (finalScore.first > finalScore.second)
            {
               result = 'B';
            }
            else
            {
               result = 'E';
            }
            return result;
         }
      }
      else {
         cout << endl << "Black has no valid moves and passes to White"  << endl;
      }
      if(reversiBoard.getValidMoves(White).size() != 0)
      {
         int playerWhite_move = reversiAgentRandom(reversiBoard, White);
         // if it is an invalid move, A automatically wins
         if(!reversiBoard.isValidMove(playerWhite_move, White))
         {
            // Let's let 'C' represent that A won because of B's invalid move
            result = 'C';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player B
         else
         {
            reversiBoard.makeMove(playerWhite_move, White);
            cout << "White: " << endl;
            reversiBoard.printBoard();
         }

         // if playerBlack made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            cout << "White: " << finalScore.second << endl;
            cout << "Black: " << finalScore.first << endl;
            // White is first in this pair
            if(finalScore.second > finalScore.first)
            {
               result = 'A';
            }
            else if (finalScore.first > finalScore.second)
            {
               result = 'B';
            }
            else
            {
               result = 'E';
            }

            return result;
         }
      }
      else {
         cout << endl << "White has no moves, and passes to Black" << endl;
      }

   }

   return result;
}

char playReversiGameMidway(Board& reversiBoard)
{
   char result = 'A'; // compiler complains without an initialization

   // repeatedly call playerA's and playerBlack's function until someone wins or makes an invalid move
   int middleOfGame = 20;
   while(!reversiBoard.isGameOver() && middleOfGame >= 0)
   {
      
      // player A goes first, if they still have valid moves,
      // Otherwise, pass to black
      if(reversiBoard.getValidMoves(Black).size() != 0)
      {
         int playerBlack_move = reversiAgentRandom(reversiBoard, Black);
         
         //if it is an invalid move, B automatically wins
         if(!reversiBoard.isValidMove(playerBlack_move, Black))
         {
            // Let's let 'D' represent that player B won because of A's invalid move
            result = 'D';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player A
         else
         {
            reversiBoard.makeMove(playerBlack_move, Black);
            middleOfGame -= 1;
            reversiBoard.printBoard();
         }

         // if playerA made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            // White is first in this pair.
            cout << "White: " << finalScore.second << endl;
            cout << "Black: " << finalScore.first << endl;
            if(finalScore.second > finalScore.first)
            {
               result = 'A';
            }
            else if (finalScore.first > finalScore.second)
            {
               result = 'B';
            }
            else
            {
               result = 'E';
            }
            return result;
         }
      }
      else {
         cout << endl << "Black has no valid moves and passes to White"  << endl;
      }
      if(reversiBoard.getValidMoves(White).size() != 0)
      {
         int playerWhite_move = reversiAgentRandom(reversiBoard, White);
         // if it is an invalid move, A automatically wins
         if(!reversiBoard.isValidMove(playerWhite_move, White))
         {
            // Let's let 'C' represent that A won because of B's invalid move
            result = 'C';
            reversiBoard.printBoard();
            return result;
         }
         // Update the board for player B
         else
         {
            reversiBoard.makeMove(playerWhite_move, White);
            reversiBoard.printBoard();
            middleOfGame -= 1;
         }

         // if playerBlack made a move that won them the game then return result
         if(reversiBoard.isGameOver())
         {
            pair<int, int> finalScore;
            finalScore = reversiBoard.getScore();
            cout << "White: " << finalScore.second << endl;
            cout << "Black: " << finalScore.first << endl;
            // White is first in this pair
            if(finalScore.second > finalScore.first)
            {
               result = 'A';
            }
            else if (finalScore.first > finalScore.second)
            {
               result = 'B';
            }
            else
            {
               result = 'E';
            }

            return result;
         }
      }
      else {
         cout << endl << "White has no moves, and passes to Black" << endl;
      }

   }

   return result;
}
int reversiAgentRandom(const Board &board, player whichPlayer)
{
   vector<int> availableMoves;
   Board copy;
   copy = board;
   availableMoves = copy.getValidMoves(whichPlayer);
   int randomMove;
   randomMove = randomInt(availableMoves.size());
   return availableMoves.at(randomMove);

}
