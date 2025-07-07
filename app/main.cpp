#include "Shtrix.hpp"
#include "Game.hpp"

#include "io.hpp"
#include <threads.h>
#include <cstdlib>
#include <termios.h>
#undef NULL
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

void init() {
   enableRawMode();
   mcsl::printf(FMT("%s\033[?25l"), shtrix::ANSI_BRIGHT_FOREGROUND_COLOR(shtrix::BLACK));
   mcsl::flush();
}
void halt() {
   std::timespec t {.tv_sec = 1, .tv_nsec = 0};
   thrd_sleep(&t, nullptr);
   mcsl::printf(FMT("\033[H\033[%uB\033[0m\033[?25h"), shtrix::BOARD_HEIGHT + 7);
   disableRawMode();
}

sint main(sint argc, char** argv) {
   init();
   uint8 level = 0;
   if (argc > 1) {
      level = mcsl::str_to_uint(argv[1], argv[1] + std::strlen(argv[1]));
      if (level > 0) {
         --level;
      }
   }
   shtrix::Game::play(level);
   halt();
}
