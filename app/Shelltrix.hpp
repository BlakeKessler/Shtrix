#ifndef SHELLTRIX_HPP
#define SHELLTRIX_HPP

#include "MCSL.hpp"

namespace shtrix {
   struct Board;
   struct Piece;
   class InputQueue;


   constexpr sint FPS = 60;
   constexpr sint BOARD_HEIGHT = 20;
   constexpr sint BOARD_WIDTH = 10;
   constexpr sint POINTS_BY_LINES_CLEARED[4] = {40, 100, 300, 1200};
   constexpr sint FRAMES_PER_CELL[30] = {
      48, 43, 38, 33, 28, 23, 18, 13,  8,  6,
       5,  5,  5,  4,  4,  4,  3,  3,  3,  2,
       2,  2,  2,  2,  2,  2,  2,  2,  2,  1
   };
   constexpr uint SOFTDROP_SPEED_FACTOR = 2;

   enum Color : uint8 {
      DEFAULT,
      BLACK,
      RED,
      GREEN,
      YELLOW,
      BLUE,
      MAGENTA,
      CYAN,
      WHITE,
   };
   const mcsl::str_slice ANSI_COLOR_CODE(Color);
};

#endif