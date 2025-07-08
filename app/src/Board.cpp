#include "Board.hpp"
#include "io.hpp"

//create new empty board
shtrix::Board::Board():
   _grid{},
   _currPiece{} {
      init();
}

//return board to the empty state
void shtrix::Board::init() {
   for (uint8 i = 0; i < BOARD_WIDTH; ++i) {
      for (uint8 j = 0; j < BOARD_HEIGHT + 4; ++j) {
         _grid[j][i] = {BLACK, false};
      }
   }
}

//print the board to the terminal
void shtrix::Board::print(uint8 level, uint32 score, uint16 lines) {
   //clear the screen and move to the top left corner
   mcsl::printf(FMT("\033[2J\033[H\033[4B\033[6C\033[s"));

   //print next piece
   mcsl::write(ANSI_BACKGROUND_COLOR(DEFAULT));
   mcsl::printf(FMT("\033[22C  NEXT  "));
   mcsl::write(ANSI_BACKGROUND_COLOR(BLACK));
   mcsl::printf(FMT("\033[8D\033[1B[][][][]"));
   mcsl::printf(FMT("\033[8D\033[1B[][][][]"));
   mcsl::printf(FMT("\033[8D\033[1B[][][][]"));
   mcsl::printf(FMT("\033[8D\033[1B[][][][]"));
   mcsl::printf(FMT("\033[4A"));
   
   mcsl::write(ANSI_BACKGROUND_COLOR(_nextPiece.c));
   for (uint8 i = 4; i--;) {
      mcsl::printf(FMT("\033[8D\033[1B"));
      uint16 mask = (_nextPiece.shape >> (4 * i)) & 0b1111;
      mcsl::printf(mask & 0b0001 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0010 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0100 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b1000 ? FMT("[]") : FMT("\033[2C"));
   }

   mcsl::printf(FMT("\033[u"));

   //print each visible row
   Color c = BLACK;
   mcsl::write(ANSI_BACKGROUND_COLOR(c));
   for (uint8 i = BOARD_HEIGHT; i--;) {
      for (uint8 j = 0; j < BOARD_WIDTH; ++j) {
         Cell curr = _grid[i][j];
         if (c != curr.c) {
            c = curr.c;
            mcsl::write(ANSI_BACKGROUND_COLOR(c));
         }
         mcsl::printf(FMT("[]"));
      }
      mcsl::printf(FMT("\033[1E\033[6C"));
   }
   mcsl::write(ANSI_BACKGROUND_COLOR(DEFAULT));
   //save bottom of grid
   mcsl::printf(FMT("\033[s"));

   //print ghost piece
   Piece ghost = _currPiece;
   sint8 ghostRow = _pieceRow;
   do {
      --ghostRow;
   } while (!checkFloorCollision(ghost, ghostRow, _pieceCol));
   ++ghostRow;
   
   mcsl::printf(FMT("\033[H\033[%uC"), 2 * (_pieceCol + 1));
   if (BOARD_HEIGHT + 2 - ghostRow) {
      mcsl::printf(FMT("\033[%uB"), BOARD_HEIGHT + 2 - ghostRow);
   }
   mcsl::write(ANSI_BACKGROUND_COLOR(BLACK));
   mcsl::write(ANSI_BRIGHT_FOREGROUND_COLOR(ghost.c));
   for (uint8 i = 4; i--;) {
      uint16 mask = (ghost.shape >> (4 * i)) & 0b1111;
      mcsl::printf(mask & 0b0001 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0010 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0100 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b1000 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(FMT("\033[8D\033[1B"));
   }

   //print the falling piece
   mcsl::printf(FMT("\033[H\033[%uC"), 2 * (_pieceCol + 1));
   if (BOARD_HEIGHT + 2 - _pieceRow) {
      mcsl::printf(FMT("\033[%uB"), BOARD_HEIGHT + 2 - _pieceRow);
   }
   mcsl::write(ANSI_BACKGROUND_COLOR(_currPiece.c));
   for (uint8 i = 4; i--;) {
      uint16 mask = (_currPiece.shape >> (4 * i)) & 0b1111;
      mcsl::printf(mask & 0b0001 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0010 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b0100 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(mask & 0b1000 ? FMT("[]") : FMT("\033[2C"));
      mcsl::printf(FMT("\033[8D\033[1B"));
   }

   //print stats
   mcsl::write(ANSI_BACKGROUND_COLOR(DEFAULT));
   mcsl::printf(FMT("\033[u"));
   mcsl::printf(FMT("%sLevel: %u\033[u\033[1B%sScore: %u\033[u\033[2B%sLines: %u%s"), ANSI_FOREGROUND_COLOR(WHITE), level, ANSI_FOREGROUND_COLOR(WHITE), score, ANSI_FOREGROUND_COLOR(WHITE), lines, ANSI_BRIGHT_FOREGROUND_COLOR(BLACK));
   mcsl::stdout.flush();
}

//drop a new piece
void shtrix::Board::newPiece(Piece p) {
   _currPiece = _nextPiece;
   _nextPiece = p;
   _pieceRow = BOARD_HEIGHT + 2;
   _pieceCol = BOARD_WIDTH / 2;
}

//calculate line clears and update the board, return linesCleared
shtrix::Board::Status shtrix::Board::runGravity() {
   //move piece down
   --_pieceRow;
   if (checkFloorCollision()) {
      goto COLLISION;
   }

   return Status{
      .linesCleared = 0,
      .didLand = false,
      .lost = false,
      .height = 0
   };

   COLLISION:
      ++_pieceRow;
      #define CHECK_CELL(row, col) \
      if((_currPiece.shape & (1 << (col + 4 * row)))) { \
         Cell& cell = _grid[_pieceRow - 2 + row][_pieceCol - 2 + col]; \
         cell.c = _currPiece.c; \
         cell.isFull = true; \
      }
      #define CHECK_ROW(row) \
      if ((_pieceRow - 2 + row >= 0) && (_currPiece.shape & (0x000F << (4 * row)))) { \
         if (_pieceRow - 2 + row >= BOARD_HEIGHT) { \
            goto LOSE; \
         } \
         CHECK_CELL(row, 0) \
         CHECK_CELL(row, 1) \
         CHECK_CELL(row, 2) \
         CHECK_CELL(row, 3) \
      }
   
      CHECK_ROW(0)
      CHECK_ROW(1)
      CHECK_ROW(2)
      CHECK_ROW(3)

      #undef CHECK_ROW
      #undef CHECK_CELL

      return lineClears();

   LOSE:
      return Status{
         .linesCleared = 0,
         .didLand = true,
         .lost = true,
         .height = 0
      };
}

shtrix::Board::Status shtrix::Board::lineClears() {
   Status status {
      .linesCleared = 0,
      .didLand = true,
      .lost = false,
      .height = 0
   };
   uint8 clearSize = 0;
   for (uint8 i = 0; i < BOARD_HEIGHT; ++i) {
      for (uint8 j = 0; j < BOARD_WIDTH; ++j) {
         if (!_grid[i][j].isFull) {
            goto NO_CLEAR;
         }
      }
      //clear
      if (!clearSize) {
         status.height = i;
      }
      ++clearSize;
      continue;
      //no clear
      NO_CLEAR:
      if (clearSize) {
         break;
      } else {
         continue;
      }
   }
   
   status.linesCleared += clearSize;
   mcsl::memcpy((ubyte*)(_grid + status.height), (ubyte*)(_grid + status.height + clearSize), (BOARD_HEIGHT - (status.height + clearSize)) * sizeof(_grid[0]));
   std::memset(_grid + BOARD_HEIGHT - clearSize, 0, sizeof(_grid[0]));
   
   return status;
}

void shtrix::Board::move(sint8 displacement) {
   //move
   _pieceCol += displacement;
   if (checkWallCollision()) {
      _pieceCol -= displacement;
   }
}

void shtrix::Board::rotR() {
   _currPiece.rotR();
   if (checkWallCollision()) {
      _currPiece.rotL();
   }
}
void shtrix::Board::rotL() {
   _currPiece.rotL();
   if (checkWallCollision()) {
      _currPiece.rotR();
   }
}

bool shtrix::Board::checkFloorCollision(Piece piece, sint8 r, sint8 c) {

   //check for collisions
   if ((r - 2 < 0) && (piece.shape & 0x000F)) {
      return true;
   }
   if ((r - 1 < 0) && (piece.shape & 0x00F0)) {
      return true;
   }
   if ((r     < 0) && (piece.shape & 0x0F00)) {
      return true;
   }
   if ((r + 1 < 0) && (piece.shape & 0xF000)) {
      return true;
   }

   #define CHECK_CELL(row, col) \
   if ((piece.shape & (1 << (col + 4 * row))) && _grid[r - 2 + row][c - 2 + col].isFull) { \
      return true; \
   }
   #define CHECK_ROW(row) \
   CHECK_CELL(row, 0) \
   CHECK_CELL(row, 1) \
   CHECK_CELL(row, 2) \
   CHECK_CELL(row, 3) \
   
   CHECK_ROW(0)
   CHECK_ROW(1)
   CHECK_ROW(2)
   CHECK_ROW(3)

   #undef CHECK_ROW
   #undef CHECK_CELL
   
   return false;
}
bool shtrix::Board::checkWallCollision(Piece piece, sint8 r, sint8 c) {
      //check for collisions
   if ((c - 2 < 0) && (piece.shape & 0x1111)) {
      return true;
   }
   if ((c - 1 < 0) && (piece.shape & 0x2222)) {
      return true;
   }
   if ((c     < 0) && (piece.shape & 0x4444)) {
      return true;
   }
   if ((c + 1 < 0) && (piece.shape & 0x8888)) {
      return true;
   }

   if ((c + 1 >= BOARD_WIDTH) && (piece.shape & 0x8888)) {
      return true;
   }
   if ((c     >= BOARD_WIDTH) && (piece.shape & 0x4444)) {
      return true;
   }
   if ((c - 1 >= BOARD_WIDTH) && (piece.shape & 0x2222)) {
      return true;
   }
   if ((c - 2 >= BOARD_WIDTH) && (piece.shape & 0x1111)) {
      return true;
   }

   #define CHECK_CELL(row, col) \
   if ((piece.shape & (1 << (col + 4 * row))) && _grid[r - 2 + row][c - 2 + col].isFull) { \
      return true; \
   }
   #define CHECK_COL(col) \
   CHECK_CELL(0, col) \
   CHECK_CELL(1, col) \
   CHECK_CELL(2, col) \
   CHECK_CELL(3, col) \
   
   CHECK_COL(0)
   CHECK_COL(1)
   CHECK_COL(2)
   CHECK_COL(3)

   #undef CHECK_ROW
   #undef CHECK_CELL

   return false;
}