#ifndef BOARD_HPP
#define BOARD_HPP

#include "Shtrix.hpp"

#include "Piece.hpp"

struct shtrix::Board {
   private:
      struct Cell {
         Color c;
         bool isFull;
      };

      Cell _grid[BOARD_HEIGHT + 4][BOARD_WIDTH];
      Piece _currPiece;
      sint8 _pieceRow;
      sint8 _pieceCol;

   public:
      struct Status {
         uint8 linesCleared;
         bool didLand;
         bool lost;
      };

      Board();
      void init();
      void print(uint8 level, uint32 score, uint16 lines);

      void newPiece(Piece p);

      Status runGravity();
      Status lineClears();

      bool checkWallCollision();
      void move(sint8 displacement);
      inline void moveLeft() { return move(-1); }
      inline void moveRight() { return move(1); }
      void rotR();
      void rotL();
};

#endif