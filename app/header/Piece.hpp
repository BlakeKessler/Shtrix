#ifndef PIECE_HPP
#define PIECE_HPP

#include "Shelltrix.hpp"

struct shtrix::Piece {
   uint16 shape;
   Color c;

   void rotR();
   void rotL();
};

namespace shtrix {
   constexpr Piece PIECES[] {
      {0x4444,    CYAN}, //LONGBAR
      {0x0226,    BLUE}, //J
      {0x0446,   WHITE}, //L
      {0x0660,  YELLOW}, //SQUARE
      {0x0360,   GREEN}, //S
      {0x0270, MAGENTA}, //T
      {0x0630,     RED}  //Z
   };
}

#endif