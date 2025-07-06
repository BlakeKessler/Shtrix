#ifndef BAG_HPP
#define BAG_HPP

#include "Shtrix.hpp"
#include "Piece.hpp"

#include "rand.hpp"

struct shtrix::Bag {
   uint8 hasI : 1 = true;
   uint8 hasJ : 1 = true;
   uint8 hasL : 1 = true;
   uint8 hasO : 1 = true;
   uint8 hasS : 1 = true;
   uint8 hasT : 1 = true;
   uint8 hasZ : 1 = true;
   const uint8 PAD : 1 = 0;

   operator bool() const {
      return *((uint8*)this);
   }
};

static_assert(sizeof(shtrix::Bag) == 1);

#endif