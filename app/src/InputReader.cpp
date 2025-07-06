#include "InputReader.hpp"

#include "io.hpp"

void shtrix::InputReader::readInputs() {
   do {
      char c = mcsl::read();
      switch (c) {
         case LEFT     :     status.left = true; break;
         case RIGHT    :    status.right = true; break;
         case ROT_LEFT :  status.rotLeft = true; break;
         case ROT_RIGHT: status.rotRight = true; break;
         case SOFT_DROP: status.softDrop = true; break;
         case HARD_DROP: status.hardDrop = true; break;

         default: break;
      }
   } while (kbhit());
}