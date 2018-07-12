#include "reversi.h"
#include <vector>

Tile::Tile(int location, int size)
{
   owner = none; 
   turn = 0;
   this->location = location;
   row = location / size;
   column = location % size;
}

Board::Board(int size)
{
   this->size = size;
   this->moveNum = 0;
   int i;
   for (i = 0; i < size * size; ++i)
   {
      BoardLayout.push_back(Tile(i, size));
   }
   int mid, topLeft, bottomLeft, topRight, bottomRight;
   mid = size / 2;
   // set up the board with 4 tiles in the middle in a diagonal fashion 
   topLeft = (mid - 1) * size + (mid - 1);
   bottomLeft = (mid - 1) * size + mid;
   topRight = mid * size + (mid - 1);
   bottomRight = mid * size + mid;
   BoardLayout[topLeft].setOwner(White);
   BoardLayout[bottomLeft].setOwner(Black);
   BoardLayout[topRight].setOwner(Black);
   BoardLayout[bottomRight].setOwner(White);
   
}

Board::Board(const Board &copy)
{
   this->BoardLayout.clear();
   this->size = copy.getSize();
   this->moveNum = copy.getTurnNum();// changed from whiteinc
   //this->moveNum = copy.getTurn(); // changed from blackinc
   this->BoardLayout = copy.getBoard();
}

Board& Board::operator=(const Board &rhs)
{
   this->BoardLayout.clear();
   this->size = rhs.getSize();
   this->moveNum = rhs.getTurnNum();
   //this->blackInc = rhs.getBlackTurn();
   this->BoardLayout = rhs.getBoard();
   return *this;
}

void Board::makeMove(int location, player mover)
{
   //cout << "In MakeMove" <<endl;
   Tile *locationTile = &(BoardLayout[location]);
   if (isValidMove(location, mover))
   {
      locationTile->setOwner(mover);
      //Set the turn for the tile
      if(mover == White)
      {
          //this->setWhiteTurn();
          //locationTile->setTurn(this->getWhiteTurn());
          this->setTurn();
          locationTile->setTurn(this->getTurnNum());
      }
      else if(mover == Black)
      {
          //this->setBlackTurn();
          //locationTile->setTurn(this->getBlackTurn());
          this->setTurn();
          locationTile->setTurn(this->getTurnNum());
      }
   }

   vector<int> flipTiles;
   Tile *flipTile;
   flipTiles = checkRight(location, mover);
   unsigned int i;
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkLeft(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkUp(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkDown(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkUpperRight(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkLowerRight(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }   
   flipTiles = checkUpperLeft(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
   flipTiles = checkLowerLeft(location, mover);
   if(flipTiles.size() != 0)
   {
      for(i = 0; i < flipTiles.size(); i++)
      {
         flipTile = &(BoardLayout[flipTiles[i]]);
         (flipTile->setOwner(mover));
      }
   }
}

bool Board::isValidMove(int location, player mover) {
   //cout << "in IsValid. location: " << location << endl;
   int column, row;
   Tile *locationTile = &(BoardLayout[location]);
   column = locationTile->getColumn();
   row = locationTile->getRow();
   if(locationTile->getOwner() != none)
   {
      //cout << location << " : tile owned" << endl;
      return false;
   }
   // Before checking every move exhaustively, make sure the potential tile
   // has a neighboring gamepiece.
   int adjacentTiles[8];
   // initialize all of adjacent tiles to -1 to represent when neighbor tiles don't exist because we have reached the edge of the board
   for(int i = 0; i < 8; i++)
   {
      adjacentTiles[i] = -1;
   }
   // Checking to see that potential move is adjacent to an enemy tile.
   // Hopefully this will allow us to skip calling those 8 functions bellow
   if(row + 1 < size)
   {
      adjacentTiles[0] = getTileLocation(row + 1, column);
      if(column + 1 < size)
         adjacentTiles[5] = getTileLocation(row + 1, column + 1);
      if(column -1 >= 0)
         adjacentTiles[7] = getTileLocation(row + 1, column - 1);
   }
   if(row - 1 >= 0)
   {
      adjacentTiles[1] = getTileLocation(row - 1, column);
      if(column - 1 >= 0)
         adjacentTiles[4] = getTileLocation(row - 1, column - 1);
      if(column + 1 < size)
         adjacentTiles[6] = getTileLocation(row - 1, column + 1);
   }
   if(column + 1 < size)
      adjacentTiles[2] = getTileLocation(row, column + 1);
   if(column - 1 >= 0)
      adjacentTiles[3] = getTileLocation(row, column - 1);
   bool neighborFound = false;
   for(int i = 0; i < 8; i++)
   {
      if(adjacentTiles[i] != -1)
      {
         if(BoardLayout[adjacentTiles[i]].getOwner() != none)
            neighborFound = true;
      }
   }
   if(!neighborFound)
   {  
      return false;
   }
   
   // Call the 8 directional functions
   vector<int> left, right, up, down, rUp, lUp, rDown, lDown;
   right = this->checkRight(location, mover);
   left = this->checkLeft(location, mover);
   up = this->checkUp(location, mover);
   down = this->checkDown(location, mover);
   rUp = this->checkUpperRight(location,mover);
   lUp = this->checkUpperLeft(location, mover);
   rDown = this->checkLowerRight(location, mover);
   lDown = this->checkLowerLeft(location, mover);
   
   /*
   cout << endl << "right size = " << right.size() << endl;
   cout << "left size = " << left.size() << endl;
   cout << "up size = " << up.size() << endl;
   cout << "rup size = " << rUp.size() << endl;
   cout << "lupsize = " << lUp.size() << endl;
   cout << "rdown size = " << rDown.size() << endl;
   cout << "ldown size = " << lDown.size() << endl << endl;*/
   
   // If any of them are non-empty, the potential move does flip game pieces
   // and is therefore valid
   if((right.size() != 0) || (left.size() != 0) || (up.size() != 0) || (down.size() != 0) || (rUp.size() != 0) || (lUp.size() != 0) || (rDown.size() != 0) || (lDown.size() != 0))
   {
      //cout << "tiles did flip" << endl;
      return true;
   }

   //cout << "Reached end of is valid" << endl;
   return false;
}

vector<int> Board::getValidMoves(player mover)
{
   int size;
   size = this->getSize();
   vector<int> validPositions;
   for(int i = 0; i < size * size; i++)
   {
      if(isValidMove(i, mover))
      {
         validPositions.push_back(i);
      }
   }
   return validPositions;
}

bool Board::isGameOver()
{
   //cout << "In isGameOver" << endl;
   Tile *locationTile;
   bool boardFull = true;
   int i;
   int size = this->getSize();
   //cout << size << " In is gameover" <<  endl;
   locationTile = NULL;
   // Check to see if all locations have been filled
   for(i = 0; i < size * size; i++)
   {
      locationTile = &(BoardLayout[i]);
      if(locationTile->getOwner() == none)
      {
         //cout << "Board not full" << endl;
         boardFull = false;
         break;
      }
   }
   if(boardFull)
   {
      //cout << "Board Full" << endl;
      return true;
   }
   //cout << "going into finding valid moves" << endl;
   for(i = 0; i < size * size; i++)
   {
      if(isValidMove(i, White) || isValidMove(i, Black))
      {
         //cout << "Valid move found" << endl;
         return false;
      }
   }
   //cout << "reached end of gameover" << endl;
   return true;
}

vector<int> Board::checkRight(int location, player mover)
{
   int size = this->getSize();
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i;
   for(i = column + 1; i < size; i++)
   {
      nextTile = &(BoardLayout[getTileLocation(row, i)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotColumn = i;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile to the right of ours.
   if(pivotTile != NULL)
   {
      // From one more than the original column to one less than the pivot column...
      for(i = column + 1; i < pivotColumn; i++)
      {
         nextTileNum  = getTileLocation(row, i);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkLeft(int location, player mover)
{
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i;
   for(i = column - 1; i >= 0; i--)
   {
      nextTile = &(BoardLayout[getTileLocation(row, i)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotColumn = i;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile to the left of ours.
   if(pivotTile != NULL)
   {
      // From one less than the original column to one more than the pivot column..
      for(i = column - 1; i >= pivotColumn + 1; i--)
      {
         nextTileNum  = getTileLocation(row, i);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkUp(int location, player mover)
{
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i;
   for(i = row - 1; i >= 0; i--)
   {
      nextTile = &(BoardLayout[getTileLocation(i, column)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile above ours.
   if(pivotTile != NULL)
   {
      // From one less than the original row to one more than the pivotRow
      for(i = row - 1; i > pivotRow; i--)
      {
         nextTileNum  = getTileLocation(i, column);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkDown(int location, player mover)
{
   int size = this->getSize();
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i;
   for(i = row + 1; i < size; i++)
   {
      nextTile = &(BoardLayout[getTileLocation(i, column)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile above ours.
   if(pivotTile != NULL)
   {
      // From one more than the original row to one less than the pivotRow
      int i;
      for(i = row + 1; i < pivotRow; i++)
      {
         nextTileNum  = getTileLocation(i, column);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkUpperRight(int location, player mover)
{
   int size = this->getSize();
   //cout << "location: " << location << endl;
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i, j;
   for(i = row - 1, j = column + 1; i >= 0 && j < size; i--, j++)
   {
      nextTile = &(BoardLayout[getTileLocation(i, j)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotColumn = j;
         pivotTile = nextTile;
         //cout << "pivotRow = " << pivotRow << endl;
         //cout << "pivotColumn = " << pivotColumn << endl;
         break;
      }
   }
   // if we found a friendly tile above and to the right of ours.
   if(pivotTile != NULL)
   {
      // From one less than the original row to one more than the pivot row
      // and from one more than the original column to one less than the pivot column
      for(i = row - 1, j = column + 1; i > pivotRow && j < pivotColumn; i--, j++)
      {
         nextTileNum  = getTileLocation(i, j);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkUpperLeft(int location, player mover)
{
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i, j;
   for(i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--)
   {
      nextTile = &(BoardLayout[getTileLocation(i,j)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotColumn = j;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile above and to the left of ours.
   if(pivotTile != NULL)
   {
      // From one less than the original row to one more than the pivot row
      // and from one less than the original column to one more than the pivot column
      for(i = row - 1, j = column - 1; i > pivotRow && j > pivotColumn; i--, j--)
      {
         nextTileNum  = getTileLocation(i, j);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkLowerRight(int location, player mover)
{
   int size = this->getSize();
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i, j;
   for(i = row + 1, j = column + 1; i < size && j < size; i++, j++)
   {
      nextTile = &(BoardLayout[getTileLocation(i,j)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotColumn = j;
         pivotTile = nextTile;
         break;
      }
  }
   // if we found a friendly tile above and to the left of ours.
   if(pivotTile != NULL)
   {
      // From one less than the original row to one more than the pivot row
      // and from one less than the original column to one more than the pivot column
      for(i = row + 1, j = column + 1; i < pivotRow && j < pivotColumn; i++, j++)
      {
         nextTileNum  = getTileLocation(i, j);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}

vector<int> Board::checkLowerLeft(int location, player mover) 
{
   int size = this->getSize();
   Tile *locationTile = &(BoardLayout[location]);
   Tile *nextTile;
   Tile *pivotTile = NULL;   // This stores the tile which is a potential ""flip end"
   int row, column, pivotColumn, pivotRow, nextTileNum;
   row = locationTile->getRow();
   column = locationTile->getColumn();
   vector<int> flipTiles;
   int i, j;
   for(i = row + 1, j = column - 1; i < size && j >= 0; i++, j--)
   {
      nextTile = &(BoardLayout[getTileLocation(i,j)]);
      // search for next Same color tile but no further
      if(nextTile->getOwner() == mover)
      {
         pivotRow = i;
         pivotColumn = j;
         pivotTile = nextTile;
         break;
      }
   }
   // if we found a friendly tile above and to the right of ours.
   if(pivotTile != NULL)
   {
      // From one less than the original row to one more than the pivot row
      // and from one more than the original column to one less than the pivot column
   
      for(i = row + 1, j = column - 1; i < pivotRow && j > pivotColumn; i++, j--)
      {
         nextTileNum  = getTileLocation(i, j);
         nextTile = &(BoardLayout[nextTileNum]);
         // If the tile is the enemy color, push back the number representing the position.
         if (nextTile->getOwner() != mover && nextTile->getOwner() != none)
         {
            flipTiles.push_back(nextTileNum);
         }
         // otherwise we have a gap so clear the vector and return it.
         else 
         {
            flipTiles.clear();
            return flipTiles;
         }
      }
   }
   return flipTiles;
}


void Board::printBoard()
{
   //int column, row, width;
   int i;
   Tile *locationTile;
   int size = this->getSize();
   //cout << size << " In PrintBoard" << endl;
   //cout << " 0  1  2  3  4  5  6  7" << endl;
   //int rowCount = 1;
   //cout << " 0 ";
   cout << endl;
   for(i = 0; i < size*size; i++)
   {
      if(i % size == 0 && i > 0)
      {
         cout << endl; //<< " " << rowCount << " ";
         //rowCount++;
      }
      locationTile = &(BoardLayout[i]);
      if (locationTile->getOwner() == White)
      {
         cout << " W ";
      }
      else if(locationTile->getOwner() == Black)
      {
         cout << " B ";
      }
      else 
      {
         cout << "   ";
      }
   }
   cout << endl;
   cout << endl;
}

int Board::getTileLocation(int row, int column) const
{
   int size = this->getSize();
   //cout << size << " : In get Tile" << endl;
   if (row < 0 || row > size || column < 0 || column > size)
      return -1;

   return size * row + column;
}

pair<int, int> Board::getScore()
{
   int i;
   pair<int, int> scores;// white and then black.
   scores.first = 0;
   scores.second = 0;
   Tile *locationTile;
   int size = this->getSize();
   for(i = 0; i < size*size; i++)
   {
     locationTile = &(BoardLayout[i]);
     if(locationTile->getOwner() == White)
     {
        scores.first++;
     }
     else if(locationTile->getOwner() == Black)
     {
        scores.second++;
     }
   }
   return scores;
}
/*
///Manual moves main;
int main()
{
   int row, column, tileNum, size;
   row = -1;
   column = -1;
   Board reversiBoard;
   reversiBoard.printBoard();
   size = reversiBoard.getSize();
   //reversiBoard->makeMove(34, White);
   //reversiBoard->printBoard();
   //cout << "Checking to see if game is over" << endl;
   while(!reversiBoard.isGameOver())
   {
      cout << "Enter row, column for player White" << endl;
      cin >> row >> column;
      tileNum = size * row + column;
      if(!reversiBoard.isValidMove(tileNum, White))
      {
         reversiBoard.printBoard();
         cout << "invalid move, black wins" << endl;
      }
      else
      {
         reversiBoard.makeMove(tileNum, White);
         reversiBoard.printBoard();
      }
      if(reversiBoard.isGameOver())
      {
         pair<int, int> finalScore;
         finalScore = reversiBoard.getScore();
         cout << "White = " << finalScore.first << endl;
         cout << "Black = " << finalScore.second << endl;
      }
      cout << "Enter row, column for player Black" << endl;
      cin >> row >> column;
      tileNum  = size*row + column;
      if(!reversiBoard.isValidMove(tileNum, Black))
      {
         reversiBoard.printBoard();
         cout << "invalid move, white wins" << endl;
      }
      else 
      {
         reversiBoard.makeMove(tileNum, Black);
         reversiBoard.printBoard();
      }
      if(reversiBoard.isGameOver())
      {
         pair<int, int> finalScore;
         finalScore = reversiBoard.getScore();
         cout << "White = " << finalScore.first << endl;
         cout << "Black = " << finalScore.second << endl;
      }
   }
   return 0;
}*/
