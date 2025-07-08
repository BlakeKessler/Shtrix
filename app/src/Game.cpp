#include "Game.hpp"

#include "io.hpp"
#include "assert.hpp"
#include "rand.hpp"
#include <threads.h>

shtrix::Game::Game(uint8 startLevel):
   board(),
   bag(),
   untilGrav(0),

   level(startLevel),
   score(0),
   linesCleared(0),
   nextLevelup(10) {

}

uint32 shtrix::Game::play(uint8 startLevel) {
   std::timespec now{};
   std::timespec_get(&now, TIME_UTC);
   mcsl::srand(now.tv_nsec);
   Game game{startLevel};
   return game.playImpl();
}
uint32 shtrix::Game::playImpl() {
   board.init();
   board.newPiece(selectPiece());
   board.newPiece(selectPiece());
   
   Board::Status status{
      .linesCleared = 0,
      .didLand = false,
      .lost = false,
      .height = 0
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
      if (input.status.pause) {
         mcsl::printf(FMT("\033[H\033[2B\033[13C%sPAUSED%s"), ANSI_FOREGROUND_COLOR(DEFAULT), ANSI_FOREGROUND_COLOR(BLACK));
         mcsl::flush();
         input.status.clear();
         std::timespec pauseTime;
         std::timespec_get(&pauseTime, TIME_UTC);
         while (!input.status.pause) {
            input.readInputs();
         }
         input.status.clear();
         std::timespec unpauseTime;
         std::timespec_get(&unpauseTime, TIME_UTC);
         nextFrame.tv_sec += unpauseTime.tv_sec - pauseTime.tv_sec;
         nextFrame.tv_nsec += unpauseTime.tv_nsec - pauseTime.tv_nsec;
         board.print(level, score, linesCleared);
         continue;
      }
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
         //apply gravity
         status = board.runGravity();
         //process results of gravity
         if (status.didLand && !status.lost) { DID_LAND:
            //delay next frame
            nextFrame.tv_nsec += NS_PER_FRAME * DELAY_FRAMES[status.height];

            //pick next piece
            board.newPiece(selectPiece());
            if (!bag) {
               new (&bag) Bag{};
            }

            //update score and lines cleared
            score += (level + 1) * (10 * POINTS_BY_LINES_CLEARED[status.linesCleared]);
            linesCleared += status.linesCleared;

            //handle levelups
            if (linesCleared >= nextLevelup) {
               ++level;
               nextLevelup += 10;
            } 
         }
         //reset gravity timer
         untilGrav = FRAMES_PER_CELL[mcsl::min(level, sizeof(FRAMES_PER_CELL) / sizeof(FRAMES_PER_CELL[0]))];
      }

      //reset inputs
      input.status.clear();

      //print
      board.print(level, score, linesCleared);

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
   debug_assert(bag && !bag.PAD);
   switch ((mcsl::rand()) % 7) {
      CASE_0:
      case 0: if (bag.hasI) { bag.hasI = false; return PIECES[0]; }
      case 1: if (bag.hasJ) { bag.hasJ = false; return PIECES[1]; }
      case 2: if (bag.hasL) { bag.hasL = false; return PIECES[2]; }
      case 3: if (bag.hasO) { bag.hasO = false; return PIECES[3]; }
      case 4: if (bag.hasS) { bag.hasS = false; return PIECES[4]; }
      case 5: if (bag.hasT) { bag.hasT = false; return PIECES[5]; }
      case 6: if (bag.hasZ) { bag.hasZ = false; return PIECES[6]; }
         goto CASE_0;
   };
   UNREACHABLE;
}