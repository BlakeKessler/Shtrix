#include "Shelltrix.hpp"
#include "Board.hpp"

#include "io.hpp"
#include <cstdlib>
#undef NULL

void init() {
   std::system("stty -echo");
   // std::system("resize -s ");
}
void halt() {
   mcsl::printf(FMT("\033[0m"));
   std::system("stty echo");
}

sint main(sint argc, char** argv) {
   init();
   shtrix::Board board;
   board.dropPiece(shtrix::PIECES[0]);
   board.runFrame();
   
   board.print();

   halt();
}