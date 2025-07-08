#include "Shtrix.hpp"
#include "Game.hpp"

#include "io.hpp"
#include <threads.h>
#include <signal.h>
#include <cstdlib>
#undef NULL

void init() {
   enableRawMode();
   mcsl::printf(FMT("%s\033[?25l"), shtrix::ANSI_BRIGHT_FOREGROUND_COLOR(shtrix::BLACK));
   mcsl::flush();
}
void halt(sint i) {
   std::timespec t {.tv_sec = 1, .tv_nsec = 0};
   thrd_sleep(&t, nullptr);
   mcsl::printf(FMT("\033[?25h\033[H\033[%uB\033[0m"), shtrix::BOARD_HEIGHT + 7);
   mcsl::flush();
   disableRawMode();
   std::exit(i);
}

sint main(sint argc, char** argv) {
   if (argc > 1) {
      signal(SIGINT, &halt);
      signal(SIGTERM, &halt);
      init();
      uint8 level = mcsl::min(mcsl::str_to_uint(argv[1], argv[1] + std::strlen(argv[1])), 29);
      shtrix::Game::play(level);
      halt(EXIT_SUCCESS);
   } else {
      mcsl::printf(FMT("\033[1mNAME\033[22m\n"));
      mcsl::printf(FMT("\t\033[4mSHTRIX\033[24m - falling tetraminos in the shell\n"));
      mcsl::printf(FMT("\033[1mSYNPOSIS\033[22m\n"));
      mcsl::printf(FMT("\tshtrix \033[1mLEVEL\033[22m\n"));
      mcsl::printf(FMT("\033[1mDESCRIPTION\033[22m\n"));
      mcsl::printf(FMT("\t\033[4mcontrols:\033[24m\n"));
      mcsl::printf(FMT("\t\t'%c': pause/unpause\n"), shtrix::PAUSE);
      mcsl::printf(FMT("\t\tESC: pause/unpause\n"));
      mcsl::printf(FMT("\t\t'%c': move left\n"), shtrix::LEFT);
      mcsl::printf(FMT("\t\t'%c': move right\n"), shtrix::RIGHT);
      mcsl::printf(FMT("\t\t'%c': rotate left\n"), shtrix::ROT_LEFT);
      mcsl::printf(FMT("\t\t'%c': rotate right\n"), shtrix::ROT_RIGHT);
      mcsl::printf(FMT("\t\t'%c': soft drop\n"), shtrix::SOFT_DROP);
      mcsl::printf(FMT("\t\t'%c': hard drop\n"), shtrix::HARD_DROP);
      mcsl::printf(FMT("\033[1mOPTIONS\033[22m\n"));
      mcsl::printf(FMT("\t\033[1mLEVEL\033[22m\n"));
      mcsl::printf(FMT("\t\tthe level to start playing at (0-29)\n"));

   }
}
