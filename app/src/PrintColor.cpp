#ifndef PRINT_COLOR_CPP
#define PRINT_COLOR_CPP

#include "Shtrix.hpp"
#include "str_slice.hpp"

const mcsl::str_slice shtrix::ANSI_COLOR_CODE(Color c) {
   switch (c) {
      case BLACK  : return mcsl::FMT("\033[40m");
      case RED    : return mcsl::FMT("\033[41m");
      case GREEN  : return mcsl::FMT("\033[42m");
      case YELLOW : return mcsl::FMT("\033[43m");
      case BLUE   : return mcsl::FMT("\033[44m");
      case MAGENTA: return mcsl::FMT("\033[45m");
      case CYAN   : return mcsl::FMT("\033[46m");
      case WHITE  : return mcsl::FMT("\033[47m");
      case DEFAULT: return mcsl::FMT("\033[49m");
   }
}

#endif