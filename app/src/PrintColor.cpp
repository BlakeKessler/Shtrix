#ifndef PRINT_COLOR_CPP
#define PRINT_COLOR_CPP

#include "Shelltrix.hpp"
#include "str_slice.hpp"

const mcsl::str_slice shtrix::ANSI_COLOR_CODE(Color c) {
   switch (c) {
      case BLACK  : return mcsl::FMT("\033[30m");
      case RED    : return mcsl::FMT("\033[31m");
      case GREEN  : return mcsl::FMT("\033[32m");
      case YELLOW : return mcsl::FMT("\033[33m");
      case BLUE   : return mcsl::FMT("\033[34m");
      case MAGENTA: return mcsl::FMT("\033[35m");
      case CYAN   : return mcsl::FMT("\033[36m");
      case WHITE  : return mcsl::FMT("\033[37m");
      case DEFAULT: return mcsl::FMT("\033[39m");
   }
}

#endif