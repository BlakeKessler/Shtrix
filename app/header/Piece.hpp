#ifndef PIECE_HPP
#define PIECE_HPP

#include "Shtrix.hpp"

struct shtrix::Piece {
   uint64 shape;
   Color c;

   void rotR();
   void rotL();

   void transpose();
   void revRows();
};

namespace shtrix {
   constexpr Piece PIECES[] {
      {0x44440F00222200F0,    CYAN}, //LONGBAR
      {0x8E0064400E2044C0,    BLUE}, //J
      {0x44600E80C4402E00,   WHITE}, //L
      {0x0660066006600660,  YELLOW}, //SQUARE
      {0xC60026400C604C80,   GREEN}, //S
      {0x4E0046400E404C40, MAGENTA}, //T
      {0x6C00462006C08C40,     RED}  //Z
   };
}

#endif