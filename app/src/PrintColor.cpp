#ifndef PRINT_COLOR_CPP
#define PRINT_COLOR_CPP

#include "Shtrix.hpp"
#include "str_slice.hpp"

const mcsl::str_slice shtrix::ANSI_BACKGROUND_COLOR(Color c) {
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
const mcsl::str_slice shtrix::ANSI_BRIGHT_BACKGROUND_COLOR(Color c) {
   switch (c) {
      case BLACK  : return mcsl::FMT("\033[100m");
      case RED    : return mcsl::FMT("\033[101m");
      case GREEN  : return mcsl::FMT("\033[102m");
      case YELLOW : return mcsl::FMT("\033[103m");
      case BLUE   : return mcsl::FMT("\033[104m");
      case MAGENTA: return mcsl::FMT("\033[105m");
      case CYAN   : return mcsl::FMT("\033[106m");
      case WHITE  : return mcsl::FMT("\033[107m");
      case DEFAULT: return mcsl::FMT("\033[49m");
   }
}
const mcsl::str_slice shtrix::ANSI_FOREGROUND_COLOR(Color c) {
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
const mcsl::str_slice shtrix::ANSI_BRIGHT_FOREGROUND_COLOR(Color c) {
   switch (c) {
      case BLACK  : return mcsl::FMT("\033[90m");
      case RED    : return mcsl::FMT("\033[91m");
      case GREEN  : return mcsl::FMT("\033[92m");
      case YELLOW : return mcsl::FMT("\033[93m");
      case BLUE   : return mcsl::FMT("\033[94m");
      case MAGENTA: return mcsl::FMT("\033[95m");
      case CYAN   : return mcsl::FMT("\033[96m");
      case WHITE  : return mcsl::FMT("\033[97m");
      case DEFAULT: return ANSI_FOREGROUND_COLOR(DEFAULT);
   }
}

#endif