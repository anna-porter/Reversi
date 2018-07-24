#include "reversi.h"
#include "neuralNet.h"
#include "evolution.h"
#include<sstream>
int main(int argc, char* argv[])
{
   Population sample;
   if(argc == 2)
   {
      string fileName = argv[1];
      sample = sample.loadPopulation(fileName);
      Organism player;
      player = sample.getOrganism(0,0);
      
      int size, row, column, tileNum;
      pair<int, int> finalScore;
      Board reversiBoard = Board(8);
      size = reversiBoard.getSize();
      
      // Human plays as white
      cout << "White: Human    Black: computer" << endl;
      while(!reversiBoard.isGameOver())
      {
         if(reversiBoard.getValidMoves(Black).size() != 0)
         {   
            int playerBlack_move = reversiAgentOneMove(reversiBoard, Black, player);
            if(!reversiBoard.isValidMove(playerBlack_move, Black))
            {
               cout << playerBlack_move << " : " << "Black made an invalid move" << endl;
               reversiBoard.printBoard();
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
            else
            {
               reversiBoard.makeMove(playerBlack_move, Black);
               reversiBoard.printBoard();
            }
            if(reversiBoard.isGameOver())
            {
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
         }
         if(reversiBoard.getValidMoves(White).size() != 0)
         {
            cout << "Enter row, column for player White" << endl;
            cin >> row >> column;
            tileNum = row * size + column;
            int playerWhite_move = tileNum;
            if(!reversiBoard.isValidMove(playerWhite_move, White))
            {
               cout << playerWhite_move << " : " << "White made an invalid move" << endl;
               reversiBoard.printBoard();
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
            else
            {
               reversiBoard.makeMove(playerWhite_move, White);
               reversiBoard.printBoard();
            }
            if(reversiBoard.isGameOver())
            {
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
         }
      }
      /*
      // Human plays as black
      cout << "White: Computer    Black: Human" << endl;
      while(!reversiBoard.isGameOver())
      {
         if(reversiBoard.getValidMoves(Black).size() != 0)
         {   
            cout << "Enter row, column for player Black" << endl;
            cin >> row >> column;
            tileNum = row * size + column;
            int playerBlack_move = tileNum;
            
            if(!reversiBoard.isValidMove(playerBlack_move, Black))
            {
               cout << playerBlack_move << " : " << "Black made an invalid move" << endl;
               reversiBoard.printBoard();
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
            else
            {
               reversiBoard.makeMove(playerBlack_move, Black);
               reversiBoard.printBoard();
            }
            if(reversiBoard.isGameOver())
            {
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
         }
         if(reversiBoard.getValidMoves(White).size() != 0)
         {
            int playerWhite_move = reversiAgentOneMove(reversiBoard, White, player);
            if(!reversiBoard.isValidMove(playerWhite_move, White))
            {
               cout << playerWhite_move << " : " << "White made an invalid move" << endl;
               reversiBoard.printBoard();
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
            else
            {
               reversiBoard.makeMove(playerWhite_move, White);
               reversiBoard.printBoard();
            }
            if(reversiBoard.isGameOver())
            {
               finalScore = reversiBoard.getScore();
               cout << "White: " << finalScore.second << endl;
               cout << "Black: " << finalScore.first << endl;
            }
         }
      }*/
   }
}
