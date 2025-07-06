#ifndef Shtrix_HPP
#define Shtrix_HPP

#include <sys/select.h>
#include <unistd.h>
#undef NULL



#include "MCSL.hpp"

namespace shtrix {
   //code from https://stackoverflow.com/a/448982
   inline bool kbhit() {
      struct timeval tv = { 0L, 0L };
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);
      return select(1, &fds, nullptr, nullptr, &tv) > 0;
   }

   struct Game;
   struct Bag;
   struct Board;
   struct Piece;

   class InputReader;

   struct Held { //separate bytes to avoid race conditions
      bool left = false;
      bool right = false;
      bool rotLeft = false;
      bool rotRight = false;
      bool softDrop = false;
      bool hardDrop = false;

      void clear() { new (this) Held(); }
   };
   constexpr char LEFT = 'a';
   constexpr char RIGHT = 'd';
   constexpr char ROT_LEFT = 'q';
   constexpr char ROT_RIGHT = 'w';
   constexpr char SOFT_DROP = 's';
   constexpr char HARD_DROP = ' ';


   constexpr uint NS_PER_FRAME = 1e9 / 60.0988; //to perfectly replicate the NES's framerate
   constexpr sint8 BOARD_HEIGHT = 20;
   constexpr sint8 BOARD_WIDTH = 10;
   constexpr sint8 POINTS_BY_LINES_CLEARED[5] = {0, 4, 10, 30, 120};
   constexpr sint8 FRAMES_PER_CELL[30] = {
      48, 43, 38, 33, 28, 23, 18, 13,  8,  6,
       5,  5,  5,  4,  4,  4,  3,  3,  3,  2,
       2,  2,  2,  2,  2,  2,  2,  2,  2,  1
   };
   constexpr sint8 SOFTDROP_SPEED_FACTOR = 2;

   enum Color : uint8 {
      BLACK,
      DEFAULT,
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