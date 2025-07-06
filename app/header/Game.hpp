#ifndef GAME_HPP
#define GAME_HPP

#include "Shtrix.hpp"
#include "Board.hpp"
#include "Bag.hpp"
#include "InputReader.hpp"

#include <ctime>
#undef NULL

struct shtrix::Game {
   private:
      InputReader input;
      Board board;
      uint32 score;
      uint16 linesCleared;
      Bag bag;
      uint8 level;
      uint8 untilGrav;
      std::timespec nextFrame;

      Game(uint8 startLevel);
      uint32 playImpl();
      Piece selectPiece();
   public:
      static uint32 play(uint8 startLevel);

};

#endif