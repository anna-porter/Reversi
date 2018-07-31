// CS 4

#ifndef REVERSI_H
#define REVERSI_H

#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <cstdlib>
#include <stdlib.h> 

using namespace std;

enum player {White, Black, none};

class Tile
{
private:
   player owner;
   int location, row, column, turn;

public:
   Tile(int location, int size);
   Tile(){}
   player getOwner() const{return owner;}
   void setOwner(player takenBy) {owner = takenBy;}
   int getTurn() const{return turn;}
   void setTurn(int newTurn) {turn = newTurn;}
   int getColumn() {return column;}
   int getRow() {return row;}
}; 

// The board for the game to be played on
class Board
{
private:
   int size, moveNum; //whiteInc, blackInc;
   vector<Tile> BoardLayout;
public:
   Board(int size = 8);
   Board(const Board &copy);
   void makeMove(int location, player mover); // Update the board with a player's move.
   bool isValidMove(int location, player mover);
   Tile getTile(int i) {return BoardLayout.at(i);}
   bool isGameOver(); 
   void printBoard();// const; 
   int getSize() const {return size;}
   int getTileLocation(int row, int column) const;
   //int getWhiteTurn() const {return moveNum;}
   int getTurnNum() const {return moveNum;}
   pair<int, int> getScore();
   //void setWhiteTurn() {moveNum += 1;} //increment turn by one
   void setTurn() {moveNum += 1;} //increment turn by one
   vector<int> getValidMoves(player);
   vector<int> checkRight(int, player);
   vector<int> checkLeft(int, player);
   vector<int> checkDown(int, player);
   vector<int> checkUp(int, player);
   vector<int> checkUpperRight(int, player);
   vector<int> checkUpperLeft(int, player);
   vector<int> checkLowerRight(int, player);
   vector<int> checkLowerLeft(int, player);
   //vector<int> checkDirection(int, player, int, int);
   vector< double> boardToInput(player mover);
   vector<Tile> getBoard() const{return BoardLayout;}

   Board& operator=(const Board &rhs);
};
int randomInt(int n);
#endif // #ifndef HEX_H
