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
      {0x00F044440F002222,    CYAN}, //LONGBAR
      {0x0223007406220170,    BLUE}, //J
      {0x0322007102260470,   WHITE}, //L
      {0x0660066006600660,  YELLOW}, //SQUARE
      {0x0132006302640630,   GREEN}, //S
      {0x0232007202620270, MAGENTA}, //T
      {0x0231003604620360,     RED}  //Z
   };
}

#endif