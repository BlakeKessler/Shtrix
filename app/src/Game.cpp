#include "Game.hpp"

#include "assert.hpp"
#include "rand.hpp"
#include <threads.h>

shtrix::Game::Game(uint8 startLevel):
   board(),
   score(0),
   linesCleared(0),
   bag(),
   level(startLevel),
   untilGrav(0) {

}

uint32 shtrix::Game::play(uint8 startLevel) {
   Game game{startLevel};
   return game.playImpl();
}
uint32 shtrix::Game::playImpl() {
   board.init();
   board.newPiece(selectPiece());
   
   Board::Status status{
      .linesCleared = 0,
      .didLand = false,
      .lost = false
   };
   while (!status.lost) {
      mcsl::rand(); //advance RNG to prevent deterministic games
      
      //calculate end time of frame
      std::timespec_get(&nextFrame, TIME_UTC);
      nextFrame.tv_nsec += NS_PER_FRAME;
      thrd_yield();

      //read inputs
      if (kbhit()) {
         input.readInputs();
      }

      //process frame
      if (input.status.left) {
         board.moveLeft();
      }
      if (input.status.right) {
         board.moveRight();
      }

      if (input.status.rotLeft) {
         board.rotL();
      }
      if (input.status.rotRight) {
         board.rotR();
      }

      if (input.status.hardDrop) {
         do {
            status = board.runGravity();
            score += 2;
         } while (!status.didLand);
         goto DID_LAND;
      }
      if (untilGrav) {
         if (input.status.softDrop) {
            untilGrav -= 2;
            if (~untilGrav) {
               untilGrav = 0;
            }
            ++score;
         } else {
            untilGrav -= 1;
         }
      }
      else { //check gravity timer
         //reset gravity timer
         untilGrav = FRAMES_PER_CELL[mcsl::min(level, sizeof(FRAMES_PER_CELL) / sizeof(FRAMES_PER_CELL[0]))];
         //apply gravity
         status = board.runGravity();
         //process results of gravity
         if (status.didLand) { DID_LAND:
            board.newPiece(selectPiece());
            if (!bag) {
               new (&bag) Bag{};
            }

            score += (level + 1) * (10 * POINTS_BY_LINES_CLEARED[status.linesCleared]);
            linesCleared += status.linesCleared;
         }
      }

      //reset inputs
      input.status.clear();

      //print
      board.print();

      //wait for next frame
      thrd_yield();
      std::timespec untilNextFrame;
      std::timespec_get(&untilNextFrame, TIME_UTC);
      untilNextFrame.tv_sec = nextFrame.tv_sec - untilNextFrame.tv_sec;
      untilNextFrame.tv_nsec = nextFrame.tv_nsec - untilNextFrame.tv_nsec;
      if (untilNextFrame.tv_nsec < 0) {
         untilNextFrame.tv_nsec += 1e9;
      }
      untilNextFrame.tv_sec = 0;
      thrd_sleep(&untilNextFrame, nullptr);
   }

   return score;
}

shtrix::Piece shtrix::Game::selectPiece() {
   //select random int between 0 and 7
   debug_assert(bag && !bag.PAD);
   switch ((mcsl::rand()) & 7) {
      CASE_0:
      case 0: if (bag.hasI) { return PIECES[0]; }
      case 1: if (bag.hasJ) { return PIECES[1]; }
      case 2: if (bag.hasL) { return PIECES[2]; }
      case 3: if (bag.hasO) { return PIECES[3]; }
      case 4: if (bag.hasS) { return PIECES[4]; }
      case 5: if (bag.hasT) { return PIECES[5]; }
      case 6: if (bag.hasZ) { return PIECES[6]; }
         goto CASE_0;

      case 7: [[clang::musttail]] return selectPiece();
   };
   UNREACHABLE;
}