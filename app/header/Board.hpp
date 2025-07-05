#ifndef BOARD_HPP
#define BOARD_HPP

#include "Shelltrix.hpp"

#include "Piece.hpp"

struct shtrix::Board {
   private:
      struct Cell {
         Color c;
         bool isFull;
      };

      Cell _grid[BOARD_HEIGHT + 4][BOARD_WIDTH];
      Piece _currPiece;
      sint _pieceRow;
      sint _pieceCol;
      sint _frame;
      sint _level;

   public:
      Board();
      void init();
      void print();
      void dropPiece(Piece p);
      uint runFrame();
};

#endif