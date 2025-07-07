#include "Piece.hpp"


void shtrix::Piece::rotR() {
   shape = __builtin_rotateright64(shape, 16);
}
void shtrix::Piece::rotL() {
   shape = __builtin_rotateleft64(shape, 16);
}