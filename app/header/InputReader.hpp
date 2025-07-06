#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include "Shtrix.hpp"

class shtrix::InputReader {
   public:
      Held status = {};

      void readInputs();
};

#endif