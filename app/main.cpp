#include "Shtrix.hpp"
#include "Game.hpp"

#include "io.hpp"
#include <threads.h>
#include <cstdlib>
#undef NULL


#include <termios.h>
void enableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void disableRawMode() {
  struct termios canon;
  tcgetattr(STDIN_FILENO, &canon);
  canon.c_lflag |= (ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &canon);
}
#undef NULL

void init() {
   enableRawMode();
   mcsl::printf(FMT("\033[90m\033[?25l"));
   mcsl::flush();
}
void halt() {
   std::timespec t {.tv_sec = 1, .tv_nsec = 0};
   thrd_sleep(&t, nullptr);
   mcsl::printf(FMT("\033[H\033[%uB\033[0m\033[?25h"), shtrix::BOARD_HEIGHT + 4);
   disableRawMode();
}

sint main(sint argc, char** argv) {
   init();
   uint score = shtrix::Game::play(10);
   halt();
   mcsl::printf(FMT("FINAL SCORE: %u!\n"), score);
   std::exit(EXIT_SUCCESS);
}